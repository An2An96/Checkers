#pragma once

#define FIELD_SIZE		8

enum E_RESULT	{
    RESULT_CELL_EMPTY,
    RESULT_STEP_AVAILABLE,
    RESULT_STEP_NOT_AVAILABLE,
    RESULT_STEP_ATTACK,
    RESULT_STEP_MUST_ATTACK,
    RESULT_STEP_CONTINUE,
    RESULT_STEP_COMPLETE,
    RESULT_GAME_CONTINUE,
    RESULT_GAME_BLACK_WIN,
    RESULT_GAME_WHITE_WIN
};

enum CheckerType
{
    C_BLACK,
    C_WHITE,
    С_BLACK_KING,
    C_WHITE_KING,
    C_NONE
};

enum COLOR  {
    WHITE,
    BLACK,
    SELECTED,
};
