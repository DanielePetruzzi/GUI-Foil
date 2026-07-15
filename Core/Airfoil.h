#pragma once

#include <QPointF>
#include <QRectF>
#include <QString>
#include <QVector>

class Airfoil
{
public:

    Airfoil();
    explicit Airfoil(const QString& name);

    // Name

    void setName(const QString& name);
    const QString& name() const;

    // Points

    void clear();

    void reserve(int size);

    void addPoint(const QPointF& point);

    void addPoint(double x, double y);

    void insertPoint(int index, const QPointF& point);

    void removePoint(int index);

    void setPoint(int index, const QPointF& point);

    QPointF point(int index) const;

    QPointF& operator[](int index);

    const QPointF& operator[](int index) const;

    QVector<QPointF>& points();

    const QVector<QPointF>& points() const;

    // Info

    int size() const;

    bool isEmpty() const;

    QRectF boundingRect() const;

    QPointF leadingEdge() const;

    QPointF trailingEdge() const;

    double chord() const;

    // Geometry

    void translate(double dx, double dy);

    void scale(double factor);

    void rotate(double angleDeg);

    void mirrorY();

private:

    QString m_name;

    QVector<QPointF> m_points;
};