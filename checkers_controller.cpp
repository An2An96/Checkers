#include "checkers_controller.h"
#include "view_core.h"

Checkers::Checkers()
{
    interface = new Window(this);
    interface->show();
}

void Checkers::NewGame(bool WhiteAI, bool BlackAI)
{
    if(game != nullptr) {
        delete game;
    }
    interface->field->ClearField();
    game = new core_checkers(WhiteAI, BlackAI);
    StepWave = 0;
    CellX = 0, CellY = 0;
    gameEnd = false;
    UpdateField();
    if(game->isAIStep())    {
        AIStep();
    }
}

void Checkers::LoadGame(const char* filename)
{
    if(game != nullptr)    {
        delete game;
    }
    interface->field->ClearField();
    game = new core_checkers(filename);
    StepWave = 0;
    CellX = 0, CellY = 0;
    gameEnd = false;
    UpdateField();
    if(game->isAIStep())    {
        AIStep();
    }
}

void Checkers::SaveGame(const char* filename)
{
    if(game == nullptr)    {
        return;
    }
    if(gameEnd) {
        interface->showWarning("Эта игра завершена, ее нельзя сохранить!");
    }
    game->Save(filename);
}

void Checkers::UpdateField()
{
    if(game == nullptr)    {
        return;
    }
    for (int y = 0; y < FIELD_SIZE; y++)	{
        for (int x = 0; x < FIELD_SIZE; x++)	{
            interface->field->SetCheckerCell(x, y, game->OnCell(x, y));
        }
    }
    char string[64];
    sprintf_s(string, "Ход: %s [%d черных, %d белых]", (game->WhoseTurn() == C_WHITE) ? "Белых" : "Черных", game->getBlacks(), game->getWhites());
    interface->setStatusBar(QString(string));
}

bool Checkers::isStartGame()
{
    return game == nullptr ? false : true;
}

bool Checkers::CheckGameStatus()
{
    int status = game->CheckGameStatus();
    if (status == RESULT_GAME_WHITE_WIN)	{
        interface->showWarning("Игра закончена, победили белые!");
        gameEnd = true;
    }
    else if (status == RESULT_GAME_BLACK_WIN)	{
        interface->showWarning("Игра закончена, победили черные!");
        gameEnd = true;
    }
    return gameEnd;
}


void Checkers::SelectedCell(int x, int y)
{
    if(game == nullptr || gameEnd || game->isAIStep())    {
        return;
    }
    if(StepWave == 0)   {   //  выбираем какой шашкой ходить
        int ability = game->AbilityMove(x, y);
        if (ability == RESULT_CELL_EMPTY)	{
            return;
        }
        else
        {
            if (game->CompareTypes(game->OnCell(x, y), game->WhoseTurn()))	{
                if (ability == RESULT_STEP_NOT_AVAILABLE)	{
                    interface->showWarning("Этой шашкой некуда ходить");
                }
                else if (game->CheckAbilityFight(x, y) == RESULT_STEP_NOT_AVAILABLE)	{
                    interface->showWarning("Есть шашка(и) которыми Вы обязаны совершить бой");
                }
                else	{
                    CellX = x, CellY = y;
                    StepWave++;
                    interface->field->SetCellSelect(x, y, true);
                }
            }
            else	{
                if(game->WhoseTurn() == C_WHITE)   {
                    interface->showWarning("Сейчас ходят: Белые");
                }
                else
                {
                    interface->showWarning("Сейчас ходят: Черные");
                }
            }
        }
    }
    else if(StepWave == 1)
    {
        int step = game->CommitStep(CellX, CellY, x, y);
        if (step == RESULT_STEP_CONTINUE)	{
            interface->field->SetCellSelect(CellX, CellY, false);
            interface->field->SetCellSelect(x, y, true);
            CellX = x, CellY = y;
        }
        else if (step == RESULT_STEP_COMPLETE)	{
            game->NextStep();
            interface->field->SetCellSelect(CellX, CellY, false);
            StepWave = 0;
        }
        else if (step == RESULT_STEP_MUST_ATTACK)	{
            interface->showWarning("Вы должны вступить в бой");
            return;
        }
        else
        {
            interface->showWarning("Такой ход невозможен, попробуйте походить иначе");
            return;
        }
        UpdateField();
        if(CheckGameStatus())   {
            return;
        }
        if(game->isAIStep())    {
            AIStep();
        }
    }
}

void Checkers::AIStep()
{
    if(CheckGameStatus())   {
        return;
    }
    int step = game->AIStep();
    if(step == RESULT_STEP_CONTINUE)    {
        UpdateField();
        AIStep();
    }
    else if(step == RESULT_STEP_COMPLETE)   {
        game->NextStep();
        UpdateField();
        if(game->isAIStep())    {
            AIStep();
        }
    }
}
