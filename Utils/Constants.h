#pragma once

#include <QString>

namespace Constants
{
    //====================================================
    // Application
    //====================================================

    inline constexpr char ApplicationName[] = "GUI Foil";

    inline constexpr int DefaultWindowWidth  = 960;
    inline constexpr int DefaultWindowHeight = 680;

    inline constexpr int DialogWidthSmall  = 300;
    inline constexpr int DialogHeightSmall = 200;

    inline constexpr int DialogWidthMedium  = 420;
    inline constexpr int DialogHeightMedium = 340;

    inline constexpr int DialogWidthLarge  = 960;
    inline constexpr int DialogHeightLarge = 760;

    //====================================================
    // Plot
    //====================================================

    inline constexpr int PlotMargin = 40;

    inline constexpr int GridDivisions = 10;

    inline constexpr int LegendFontSize = 8;

    inline constexpr int AxisFontSize = 9;

    inline constexpr double PlotPaddingFactor = 0.25;

    inline constexpr double MinimumVerticalPadding = 0.01;

    //====================================================
    // Airfoil
    //====================================================

    inline constexpr int DefaultAirfoilSamples = 200;

    inline constexpr int MorphSamples = 300;

    inline constexpr int CatmullRomSamples = 300;

    inline constexpr int MaximumControlPoints = 19;

    //====================================================
    // Numerical
    //====================================================

    inline constexpr double NumericalTolerance = 1e-12;

    inline constexpr double LinearSolverTolerance = 1e-14;

    inline constexpr int BinarySearchIterations = 60;

    //====================================================
    // XFoil
    //====================================================

    inline constexpr int XFoilStartupTimeout = 5000;

    inline constexpr int XFoilExecutionTimeout = 60000;

    inline constexpr int XFoilKillTimeout = 3000;

    //====================================================
    // Default Values
    //====================================================

    inline constexpr double DefaultAoA = 0.0;

    inline constexpr double DefaultMach = 0.0;

    inline constexpr double DefaultReynolds = 0.0;

    inline constexpr double DefaultThickness = 0.12;

    inline constexpr double DefaultXT = 0.30;

    inline constexpr double DefaultC1 = 0.30;

    inline constexpr double DefaultC2 = 0.70;

    inline constexpr double DefaultC3 = 0.05;

    inline constexpr double DefaultC4 = 0.00;

    inline constexpr double DefaultRho = 1.00;

    inline constexpr double DefaultBeta = 1.00;

    inline constexpr double DefaultFlapX = 0.75;

    inline constexpr double DefaultFlapY = 0.00;

    inline constexpr double DefaultFlapAngle = 0.00;

    //====================================================
    // Directories
    //====================================================

    inline constexpr char TempDirectory[] = "temp";

    inline constexpr char CurrentAirfoilFile[] = "CurrentAirfoil.dat";

    inline constexpr char PreviousAirfoilFile[] = "PreviousAirfoil.dat";

    inline constexpr char XFoilInstructionFile[] = "xfoil_instr.txt";

    inline constexpr char CurrentOutputFile[] = "cur.dat";

    inline constexpr char TemporaryFile[] = "tmp.dat";

    inline constexpr char TemporaryRepanelFile[] = "tmprep.dat";

#ifdef Q_OS_WIN
    inline constexpr char XFoilExecutable[] = "xfoil.exe";
#else
    inline constexpr char XFoilExecutable[] = "xfoil";
#endif

}