#include "checkers_figure.h"

Checkers_figure::Checkers_figure(CheckerType type)
{
    Type = type;
}

CheckerType Checkers_figure::GetType()
{
    return Type;
}

void Checkers_figure::isSetKing(int y)	{
    if (Type == C_BLACK && y == 0)	{
        Type = С_BLACK_KING;
    }
    else if(Type == C_WHITE && y == (FIELD_SIZE - 1)) {
        Type = C_WHITE_KING;
    }
}

bool Checkers_figure::isKing()	{
    if(Type == С_BLACK_KING || Type == C_WHITE_KING)    {
        return true;
    }
    return false;
}
