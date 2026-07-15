#pragma once

#include <QColor>

namespace Plot_Colors
{
    //====================================================
    // Background
    //====================================================

    inline const QColor Background      = QColor("#FFFFFF");
    inline const QColor Border          = QColor("#D1D5DB");

    //====================================================
    // Grid
    //====================================================

    inline const QColor Grid            = QColor("#E5E7EB");
    inline const QColor GridText        = QColor("#6B7280");
    inline const QColor Axis            = QColor("#9CA3AF");

    //====================================================
    // Airfoils
    //====================================================

    inline const QColor CurrentAirfoil  = QColor("#2563EB");
    inline const QColor PreviousAirfoil = QColor("#9CA3AF");

    //====================================================
    // Pressure Distribution
    //====================================================

    inline const QColor CurrentCp       = QColor("#DC2626");
    inline const QColor PreviousCp      = QColor("#F87171");

    //====================================================
    // Lift Vector
    //====================================================

    inline const QColor CurrentLift     = QColor("#059669");
    inline const QColor PreviousLift    = QColor("#6EE7B7");

    //====================================================
    // Morph Editor
    //====================================================

    inline const QColor MorphGhost      = QColor("#94A3B8");
    inline const QColor ControlPoint    = QColor("#2563EB");
    inline const QColor SelectedPoint   = QColor("#FACC15");

    //====================================================
    // Flap Preview
    //====================================================

    inline const QColor Hinge           = QColor("#EA580C");
    inline const QColor FlapDirection   = QColor("#7C3AED");

    //====================================================
    // Generic
    //====================================================

    inline const QColor Success         = QColor("#16A34A");
    inline const QColor Warning         = QColor("#D97706");
    inline const QColor Error           = QColor("#DC2626");
}