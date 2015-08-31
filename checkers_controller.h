#pragma once

#include "checkers_data.h"
#include "checkers_core.h"
#include "view_core.h"

class Checkers  {

public:
    Checkers();

    void NewGame(bool, bool);
    void LoadGame(const char*);
    void SaveGame(const char*);
    void UpdateField();
    bool isStartGame();
    void SelectedCell(int, int);

private:
    void AIStep();
    bool CheckGameStatus();

    bool gameEnd = false;
    int StepWave = 0;
    int CellX, CellY;

    Window* interface = nullptr;
    core_checkers* game = nullptr;
};
