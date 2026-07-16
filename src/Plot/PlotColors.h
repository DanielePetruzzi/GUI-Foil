#pragma once

#include <QColor>
#include <Qt>

namespace PlotColors {

// Airfoil
static const QColor curFoil   ("#2563EB");   // vivid blue
static const QColor prevFoil  ("#93C5FD");   // light blue

// Cp
static const QColor curCp     ("#DC2626");   // vivid red
static const QColor prevCp    ("#FCA5A5");   // light red

// Lift arrow
static const QColor curLift   ("#16A34A");   // vivid green
static const QColor prevLift  ("#86EFAC");   // light green

// Morph original
static const QColor morphGhost("#9CA3AF");   // neutral grey

// Hinge / flap
static const QColor hingeV    ("#EF4444");   // red vertical
static const QColor flapDir   ("#F97316");   // orange arrow

// Grid
static const QColor grid      ("#E5E7EB");
static const QColor gridText  ("#374151");

// Background
static const QColor bg        (Qt::white);

} // namespace PlotColors
