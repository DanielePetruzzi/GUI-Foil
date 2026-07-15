#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

namespace Plot_Colors
{

// Pens

QPen axesPen();

QPen gridPen();

QPen airfoilPen();

QPen referenceAirfoilPen();

QPen splinePen();

QPen selectedSplinePen();

QPen constructionPen();

QPen highlightPen();

QPen selectionRectanglePen();

// Brushes

QBrush controlPointBrush();

QBrush selectedPointBrush();

QBrush leadingEdgeBrush();

QBrush trailingEdgeBrush();

QBrush backgroundBrush();

}