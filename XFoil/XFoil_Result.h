#pragma once

#include <QString>

class XFoil_Result
{
public:

    XFoil_Result();

    bool success;

    QString errorMessage;

    QString standardOutput;

    QString standardError;
};