#include "Plot_Colors.h"

namespace Plot_Colors
{

QPen axesPen()
{
    return QPen(Qt::black,1);
}

QPen gridPen()
{
    return QPen(QColor(225,225,225),0);
}

QPen airfoilPen()
{
    return QPen(Qt::blue,2);
}

QPen referenceAirfoilPen()
{
    return QPen(QColor(150,150,150),1,Qt::DashLine);
}

QPen splinePen()
{
    return QPen(QColor(0,140,255),2);
}

QPen selectedSplinePen()
{
    return QPen(Qt::red,2);
}

QPen constructionPen()
{
    return QPen(QColor(0,170,0),1,Qt::DashDotLine);
}

QPen highlightPen()
{
    return QPen(QColor(255,128,0),2);
}

QPen selectionRectanglePen()
{
    return QPen(Qt::darkGray,1,Qt::DashLine);
}

QBrush controlPointBrush()
{
    return QBrush(Qt::blue);
}

QBrush selectedPointBrush()
{
    return QBrush(Qt::red);
}

QBrush leadingEdgeBrush()
{
    return QBrush(Qt::green);
}

QBrush trailingEdgeBrush()
{
    return QBrush(Qt::darkYellow);
}

QBrush backgroundBrush()
{
    return QBrush(Qt::white);
}

}