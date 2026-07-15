#include "XFoil_Settings.h"

XFoil_Settings::XFoil_Settings()
{
    reynolds = 1000000.0;
    mach = 0.0;
    nCrit = 9.0;

    iterations = 100;

    viscous = true;

    repanel = true;

    panelCount = 160;

    panelLE = 0.85;
    panelTE = 0.15;
}