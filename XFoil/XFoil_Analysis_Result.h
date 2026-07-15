#pragma once

class XFoil_Analysis_Result
{
public:

    XFoil_Analysis_Result();

    bool converged;

    double alpha;

    double cl;

    double cd;

    double cdp;

    double cm;

    double xtrTop;

    double xtrBottom;
};