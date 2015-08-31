#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>

#include <QApplication>
#include <QTime>
#include <QDebug>

#include "utils.h"
#include "checkers_data.h"
#include "checkers_figure.h"

class core_checkers
{
public:
    core_checkers(bool, bool);
    core_checkers(const char*);

    bool Save(const char* filename);
    int CheckGameStatus();

    int CommitStep(int, int, int, int);
    int AbilityMove(int, int, std::vector<int>* = nullptr, std::vector<int>* = nullptr);
    int CheckAbilityFight(int, int);
    void NextStep();
    int AIStep();

    int getWhites();
    int getBlacks();

    bool isAIStep();
    CheckerType WhoseTurn();
    CheckerType OnCell(int, int);

    int CompareTypes(int, int);

private:
    bool CreateChecker(int, CheckerType);
    bool DeleteChecker(int);


    int CommitStepKing(int, int, int, int);
    int AbilityMoveKing(int, int, std::vector<int>* = nullptr, std::vector<int>* = nullptr);
    int CheckKingMoves(int, int, int, int);


    int Cell(int, int);
    void Coord(int, int&, int&);

    CheckerType Step = C_WHITE;
    int blackCheckers = 0, whiteCheckers = 0;
    bool BlackAI = false, WhiteAI = false;
    std::map < int, std::unique_ptr<Checkers_figure> > gField;
};
