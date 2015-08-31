#pragma once

#include "checkers_data.h"

class Checkers_figure
{
public:
    Checkers_figure(CheckerType);

    CheckerType GetType();

    void isSetKing(int);
    bool isKing();
    void SetKing();

private:
    CheckerType Type;
};
