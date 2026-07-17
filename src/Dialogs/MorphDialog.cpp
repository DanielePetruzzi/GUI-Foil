#include "MorphDialog.h"
#include "ChildDialogFactory.h"
#include "../Geometry/CatmullRom.h"
#include "../Plot/PlotTransform.h"
#include "../Plot/PlotColors.h"
#include "../Utils/DatFile.h"
#include "../Utils/FileUtils.h"
#include "../XFoil/AirfoilFileRotation.h"
#include "../Widgets/ResizableLabel.h"
#include "../Widgets/WindowUtils.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QMouseEvent>
#include <QRegularExpression>
#include <memory>
#include <cmath>

namespace Dialogs {
namespace {

// Handles dragging of morph control points on the canvas. Reads/writes the
// shared control-point vector and the shared PlotTransform kept in sync by
// the redraw lambda in showMorphDialog().
class MorphMouseFilter : public QObject {
public:
    MorphMouseFilter(QLabel *canvas,
                      std::shared_ptr<QVector<QPointF>> controlPts,
                      std::shared_ptr<PlotTransform> transform,
                      std::shared_ptr<int> dragIndex,
                      std::function<void()> redraw)
        : QObject(canvas), m_canvas(canvas), m_controlPts(controlPts),
          m_transform(transform), m_dragIndex(dragIndex), m_redraw(std::move(redraw))
    {}

    bool eventFilter(QObject *, QEvent *e) override
    {
        static constexpr double hitRadiusPx = 10.0;

        if (e->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent *>(e);
            const QPointF worldPos = m_transform->toWorld(WindowUtils::mouseEventPos(me));
            const double scale = m_transform->scale();

            for (int i = 0; i < m_controlPts->size(); ++i) {
                const double dx = (*m_controlPts)[i].x() - worldPos.x();
                const double dy = (*m_controlPts)[i].y() - worldPos.y();
                if (std::sqrt(dx * dx + dy * dy) * scale < hitRadiusPx) {
                    *m_dragIndex = i;
                    return true;
                }
            }
        }

        if (e->type() == QEvent::MouseButtonRelease) {
            *m_dragIndex = -1;
            m_redraw();
            return true;
        }

        if (e->type() == QEvent::MouseMove && *m_dragIndex >= 0) {
            auto *me = static_cast<QMouseEvent *>(e);
            (*m_controlPts)[*m_dragIndex] = m_transform->toWorld(WindowUtils::mouseEventPos(me));
            m_redraw();
            return true;
        }

        return false;
    }

private:
    QLabel *m_canvas;
    std::shared_ptr<QVector<QPointF>> m_controlPts;
    std::shared_ptr<PlotTransform> m_transform;
    std::shared_ptr<int> m_dragIndex;
    std::function<void()> m_redraw;
};

} // namespace

void showMorphDialog(QWidget *owner, const std::function<void()> &redraw)
{
    const QString tempDir = AppPaths::tempDir();
    const QString curPath = AppPaths::currentAirfoilDat();

    const DatFile::AirfoilData current = DatFile::load(curPath);
    if (current.isEmpty())
        return;

    const QVector<QPointF> &originalPts = current.points;
    auto originalPtsPtr = std::make_shared<QVector<QPointF>>(originalPts);

    // Arc-length parameterization of the loaded outline.
    const int N = originalPts.size();
    QVector<double> arc(N, 0.0);
    for (int i = 1; i < N; ++i) {
        const double dx = originalPts[i].x() - originalPts[i - 1].x();
        const double dy = originalPts[i].y() - originalPts[i - 1].y();
        arc[i] = arc[i - 1] + std::sqrt(dx * dx + dy * dy);
    }
    const double totalLen = arc.last();
    for (auto &a : arc) a /= totalLen;

    auto interpAt = [&](double t) -> QPointF {
        if (t <= 0) return originalPts.first();
        if (t >= 1) return originalPts.last();
        for (int i = 1; i < N; ++i) {
            if (arc[i] >= t) {
                const double a = (t - arc[i - 1]) / (arc[i] - arc[i - 1]);
                return QPointF(originalPts[i - 1].x() + a * (originalPts[i].x() - originalPts[i - 1].x()),
                               originalPts[i - 1].y() + a * (originalPts[i].y() - originalPts[i - 1].y()));
            }
        }
        return originalPts.last();
    };

    static const QVector<double> tCtrl = {
        0, 0.04, 0.08, 0.14, 0.20, 0.27, 0.34, 0.40,
        0.46, 0.50, 0.54, 0.60, 0.66, 0.73, 0.80,
        0.86, 0.92, 0.96, 1.0
    };

    QVector<QPointF> controlPts;
    for (double t : tCtrl) controlPts.append(interpAt(t));
    auto controlPtsPtr = std::make_shared<QVector<QPointF>>(controlPts);

    QDialog *dlg = new QDialog(owner);
    dlg->setWindowTitle("Morph Airfoil");
    setupChildDialog(owner, dlg, 820, 520);

    QVBoxLayout *mainLay = new QVBoxLayout(dlg);

    ResizableLabel *canvas = new ResizableLabel;
    canvas->setMinimumSize(600, 340);
    canvas->setStyleSheet("background-color: white; border: 1px solid #D1D5DB;");
    canvas->setMouseTracking(true);
    mainLay->addWidget(canvas, 1);

    QPushButton *saveBtn = new QPushButton("Save Morphed Airfoil");
    mainLay->addWidget(saveBtn);

    auto dragIndex = std::make_shared<int>(-1);
    auto transform = std::make_shared<PlotTransform>();

    auto redrawCanvas = [=]() {
        auto &pts = *originalPtsPtr;
        auto &ctrl = *controlPtsPtr;
        const int W = canvas->width(), H = canvas->height();
        if (W <= 0 || H <= 0) return;
        const int margin = 40;

        double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
        for (auto &p : pts) {
            minX = qMin(minX, p.x()); maxX = qMax(maxX, p.x());
            minY = qMin(minY, p.y()); maxY = qMax(maxY, p.y());
        }
        const double pad = (maxY - minY) * 0.4;
        minY -= pad; maxY += pad;

        transform->fit(minX, maxX, minY, maxY, W, H, margin);

        QPixmap px(W, H);
        px.fill(PlotColors::bg);
        QPainter pa(&px);
        pa.setRenderHint(QPainter::Antialiasing);

        PlotUtils::drawSimpleGrid(pa, *transform);

        // original ghost
        pa.setPen(QPen(PlotColors::morphGhost, 1.2, Qt::DashLine));
        pa.drawPolyline(QPolygonF(PlotUtils::mapPolyline(*transform, pts)));

        // morphed curve
        const QVector<QPointF> morphed = Geometry::catmullRomSpline(ctrl, 300);
        pa.setPen(QPen(PlotColors::curFoil, 2));
        pa.drawPolyline(QPolygonF(PlotUtils::mapPolyline(*transform, morphed)));

        // control points
        for (int i = 0; i < ctrl.size(); ++i) {
            const QPointF sp = transform->toScreen(ctrl[i]);
            pa.setPen(PlotColors::curCp);
            pa.setBrush(i == *dragIndex ? Qt::yellow : PlotColors::curCp);
            pa.drawEllipse(sp, 5, 5);
        }

        // legend
        int lx = margin, ly = margin - 10;
        PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::morphGhost, Qt::DashLine, "Original");
        PlotUtils::drawLegendEntry(pa, lx, ly, PlotColors::curFoil, Qt::SolidLine, "Morphed");

        canvas->setPixmap(px);
    };

    canvas->onResize = [redrawCanvas](int, int) { redrawCanvas(); };

    auto *filter = new MorphMouseFilter(canvas, controlPtsPtr, transform, dragIndex, redrawCanvas);
    canvas->installEventFilter(filter);

    redrawCanvas();

    QObject::connect(saveBtn, &QPushButton::clicked, dlg, [=]() {
        const QVector<QPointF> &ctrl = *controlPtsPtr;

        XFoil::rotateCurrentToPrevious(tempDir);

        const QVector<QPointF> morphed = Geometry::catmullRomSpline(ctrl, 300);

        QString base = current.name;
        base.remove(QRegularExpression("\\s+MORPHED\\s+\\d+$"));
        int idx = 1;
        const auto m = QRegularExpression("MORPHED\\s+(\\d+)$").match(current.name);
        if (m.hasMatch()) idx = m.captured(1).toInt() + 1;

        DatFile::save(curPath, base + " MORPHED " + QString::number(idx), morphed);

        redraw();
        dlg->accept();
    });

    dlg->show();
}

} // namespace Dialogs
