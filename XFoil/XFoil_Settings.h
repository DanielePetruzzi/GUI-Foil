#pragma once

class XFoil_Settings
{
public:

    XFoil_Settings();

    double reynolds;
    double mach;
    double nCrit;

    int iterations;

    bool viscous;
    bool repanel;

    int panelCount;

    double panelLE;
    double panelTE;
};