#include "checkers_core.h"

core_checkers::core_checkers(bool WhiteAI, bool BlackAI)
{
    //	Белые
    for (int y = 0, k = 0; y < FIELD_SIZE; y++)	{
        for (int x = IsEven(y + 1) ? 1 : 0; x < FIELD_SIZE; x += 2)	{
            CreateChecker(Cell(x, y), C_WHITE);
            if (++k == 12)	break;
        }
        if (k == 12)	break;
    }
    //	Черные
    for (int y = FIELD_SIZE - 1, k = 0; y >= 0; y--)	{
        for (int x = IsEven(y + 1) ? 7 : 6; x >= 0; x -= 2)	{
            CreateChecker(Cell(x, y), C_BLACK);
            if (++k == 12)	break;
        }
        if (k == 12)	break;
    }

    this->BlackAI = BlackAI, this->WhiteAI = WhiteAI;
}

core_checkers::core_checkers(const char* filename)
{
    std::ifstream in(filename);
    if (!in.is_open())	{
        std::cerr << "Не удалось открыть файл" << std::endl;
        return;
    }
    int curStep = 0, whiteAI = 0, blackAI = 0, Cell = 0, Type = 0;
    in >> curStep >> whiteAI >> blackAI;
    Step = (CheckerType)curStep;
    this->BlackAI = blackAI, this->WhiteAI = whiteAI;
    std::cout << Step << std::endl;
    while (in.eof() == false)	{
        in >> Cell >> Type;
        CreateChecker(Cell, (CheckerType)Type);
    }
    in.close();
}


bool core_checkers::Save(const char* filename)
{
    std::ofstream out(filename);
    if (!out.is_open())	{
        return false;
    }
    out << Step << " " << WhiteAI << " " << BlackAI << '\n';
    for (auto i = gField.begin(); i != gField.end(); ++i)
    {
        if (i->second == nullptr)   {
            continue;
        }
        out << i->first << " " << i->second->GetType() << '\n';
    }
    out.close();
    return true;
}

bool core_checkers::CreateChecker(int cell, CheckerType type)	{
    if (gField[cell])	return false;


    if (CompareTypes(C_BLACK, type))		blackCheckers++;
    else if (CompareTypes(C_WHITE, type))	whiteCheckers++;
    else	return false;

    gField[cell] = std::make_unique<Checkers_figure>(type);
    return true;
}

bool core_checkers::DeleteChecker(int cell)	{
    if (gField[cell])	{
        if (CompareTypes(C_BLACK, gField[cell]->GetType()))	blackCheckers--;
        else                                                whiteCheckers--;

        gField.erase(cell);
    }
    return false;
}

int core_checkers::getWhites()  {
    return whiteCheckers;
}

int core_checkers::getBlacks()  {
    return blackCheckers;
}

CheckerType core_checkers::WhoseTurn()
{
    return Step;
}

bool core_checkers::isAIStep()
{
    if((Step == C_BLACK && BlackAI) || (Step == C_WHITE && WhiteAI))    {
        return true;
    }
    return false;
}

CheckerType core_checkers::OnCell(int x, int y)
{
    if (gField[Cell(x, y)])	{
        return gField[Cell(x, y)]->GetType();
    }
    return C_NONE;
}

int core_checkers::CheckGameStatus()	{
    if (blackCheckers == 0 && whiteCheckers)		return RESULT_GAME_WHITE_WIN;
    else if (blackCheckers && whiteCheckers == 0)	return RESULT_GAME_BLACK_WIN;
    return RESULT_GAME_CONTINUE;
}

void core_checkers::NextStep()
{
    if(Step == C_BLACK) {
        Step = C_WHITE;
    }
    else if(Step == C_WHITE)    {
        Step = C_BLACK;
    }
}

int core_checkers::AbilityMove(int x, int y, std::vector<int>* attack, std::vector<int>* steps)
{
    if (gField[Cell(x, y)] == nullptr)	{
        return RESULT_CELL_EMPTY;
    }
    else if (gField[Cell(x, y)]->isKing())	{
        return AbilityMoveKing(x, y, attack, steps);
    }
    bool stepAvailable = false, stepAttack = false;
    for (int j = y - 1; j <= y + 1; j++){
        for (int i = x - 1; i <= x + 1; i++)	{
            if(
                i < 0 || i >= FIELD_SIZE || j < 0 || j >= FIELD_SIZE || (i == x && j == y) ||
                IsEven(j) != IsEven(i))	{
                continue;
            }
            if (gField[Cell(i, j)] == nullptr)	{
                if (CompareTypes(C_BLACK, gField[Cell(x, y)]->GetType()) && j > y ||
                    CompareTypes(C_WHITE, gField[Cell(x, y)]->GetType()) && j < y)	{
                    continue;
                }
                if(steps)   steps->push_back(Cell(i, j));
                stepAvailable = true;
            }
            else if (CompareTypes(gField[Cell(x, y)]->GetType(), gField[Cell(i, j)]->GetType()))	{
                continue;
            }
            else
            {
                int bufX = x < i ? i + 1 : i - 1,
                    bufY = y < j ? j + 1 : j - 1;
                if (bufX >= 0 && bufX < FIELD_SIZE && bufY >= 0 && bufY < FIELD_SIZE && gField[Cell(bufX, bufY)] == nullptr)	{
                    if(attack)   attack->push_back(Cell(bufX, bufY));
                    stepAttack = true;
                }
            }
        }
    }
    if(stepAttack)  {
        return RESULT_STEP_ATTACK;
    }
    else    {
        return stepAvailable ? RESULT_STEP_AVAILABLE : RESULT_STEP_NOT_AVAILABLE;
    }
}

int core_checkers::CheckKingMoves(int x, int y, int i, int j)	{
    if (gField[Cell(i, j)] == nullptr)	{
        return RESULT_STEP_AVAILABLE;
    }
    else if (CompareTypes(gField[Cell(i, j)]->GetType(), gField[Cell(x, y)]->GetType()) == false)
    {
        int bufX = x < i ? i + 1 : i - 1,
            bufY = y < j ? j + 1 : j - 1;
        if (bufX >= 0 && bufX < FIELD_SIZE && bufY >= 0 && bufY < FIELD_SIZE && gField[Cell(bufX, bufY)] == nullptr)	{
            return RESULT_STEP_ATTACK;
        }
    }
    return RESULT_STEP_NOT_AVAILABLE;
}

int core_checkers::AbilityMoveKing(int x, int y, std::vector<int>* attack, std::vector<int>* steps)
{
    bool stepAvailable = false, stepAttack = false;
    int result = 0;

    //	проверяем правое нижнее направление
    for (int i = 1; (x + i >= 0 && x + i < FIELD_SIZE && y + i >= 0 && y + i < FIELD_SIZE); i++)	{
        result = CheckKingMoves(x, y, x + i, y + i);
        if (result == RESULT_STEP_AVAILABLE)
        {
            stepAvailable = true;
            if(steps)   steps->push_back(Cell(x + i, y +  i));
        }
        else if (result == RESULT_STEP_ATTACK)
        {
            stepAttack = true;
            if(attack)   attack->push_back(Cell(x + i + 1, y +  i + 1));
            break;
        }
        else break;
    }
    //	проверяем левое нижнее направление
    for (int i = 1; (x - i >= 0 && x - i < FIELD_SIZE && y + i >= 0 && y + i < FIELD_SIZE); i++)	{
        result = CheckKingMoves(x, y, x - i, y + i);
        if (result == RESULT_STEP_AVAILABLE)
        {
            stepAvailable = true;
            if(steps)   steps->push_back(Cell(x - i, y +  i));
        }
        else if (result == RESULT_STEP_ATTACK)
        {
            stepAttack = true;
            if(attack)   attack->push_back(Cell(x - i - 1, y +  i + 1));
            break;
        }
        else break;
    }
    //	проверяем правое верхнее направление
    for (int i = 1; (x + i >= 0 && x + i < FIELD_SIZE && y - i >= 0 && y - i < FIELD_SIZE); i++)	{
        result = CheckKingMoves(x, y, x + i, y - i);
        if (result == RESULT_STEP_AVAILABLE)
        {
            stepAvailable = true;
            if(steps)   steps->push_back(Cell(x + i, y -  i));
        }
        else if (result == RESULT_STEP_ATTACK)
        {
            stepAttack = true;
            if(attack)   attack->push_back(Cell(x + i + 1, y -  i - 1));
            break;
        }
        else break;
    }
    //	проверяем левое верхнее направление
    for (int i = 1; (x - i >= 0 && x - i < FIELD_SIZE && y - i >= 0 && y - i < FIELD_SIZE); i++)	{
        result = CheckKingMoves(x, y, x - i, y - i);
        if (result == RESULT_STEP_AVAILABLE)
        {
            stepAvailable = true;
            if(steps)   steps->push_back(Cell(x - i, y -  i));
        }
        else if (result == RESULT_STEP_ATTACK)
        {
            stepAttack = true;
            if(attack)   attack->push_back(Cell(x - i - 1, y -  i - 1));
            break;
        }
        else break;
    }
    if(stepAttack)  {
        return RESULT_STEP_ATTACK;
    }
    else    {
        return stepAvailable ? RESULT_STEP_AVAILABLE : RESULT_STEP_NOT_AVAILABLE;
    }
}

int core_checkers::CheckAbilityFight(int x, int y)	{
    if (gField[Cell(x, y)] == nullptr)	{
        return RESULT_CELL_EMPTY;
    }
    int ability = AbilityMove(x, y);
    CheckerType type = gField[Cell(x, y)]->GetType();

    if (ability == 1)	{
        int bX, bY;
        for (auto i = gField.begin(); i != gField.end(); i++)
        {
            if (i->second == nullptr)	continue;
            if (CompareTypes(type, i->second->GetType()))
            {
                Coord(i->first, bX, bY);
                if (AbilityMove(bX, bY) == RESULT_STEP_ATTACK)	{
                    return RESULT_STEP_NOT_AVAILABLE;
                }
            }
        }
    }
    return RESULT_STEP_AVAILABLE;
}

int core_checkers::CommitStep(int x, int y, int toX, int toY)
{
    if (IsEven(toY) != IsEven(toX) || gField[Cell(toX, toY)])	{
        return RESULT_STEP_NOT_AVAILABLE;
    }
    if (gField[Cell(x, y)]->isKing())	{
        return CommitStepKing(x, y, toX, toY);
    }
    CheckerType type = gField[Cell(x, y)]->GetType();

    int difX = abs(x - toX), difY = abs(y - toY);
    if (difX < 3 && difY < 3)	{
        //	#ход в соседнюю клетку
        if (difX < 2 && difY < 2)	{
            if (AbilityMove(x, y) != RESULT_STEP_ATTACK)	{	//	условие для того чтобы бить по возможность было обязательно
                if (type == C_BLACK && y >= toY)	{
                    gField[Cell(toX, toY)] = std::move(gField[Cell(x, y)]);
                    gField[Cell(toX, toY)]->isSetKing(toY);
                    return RESULT_STEP_COMPLETE;
                }
                else if (type == C_WHITE && y <= toY)	{
                    gField[Cell(toX, toY)] = std::move(gField[Cell(x, y)]);
                    gField[Cell(toX, toY)]->isSetKing(toY);
                    return RESULT_STEP_COMPLETE;
                }
            }
            else
            {
                return RESULT_STEP_MUST_ATTACK;
            }
        }
        //	#ход с поеданием шашки
        else if (difX > 1 && difY > 1)
        {
            int bufX = x < toX ? x + 1 : x - 1,
                bufY = y < toY ? y + 1 : y - 1;
            CheckerType t = gField[Cell(bufX, bufY)]->GetType();
            if (t != C_NONE && CompareTypes(t, type) == false)	{
                gField[Cell(toX, toY)] = std::move(gField[Cell(x, y)]);
                DeleteChecker(Cell(bufX, bufY));
                gField[Cell(toX, toY)]->isSetKing(toY);
                if (AbilityMove(toX, toY) != RESULT_STEP_ATTACK)	{
                    return RESULT_STEP_COMPLETE;
                }
                else
                {
                    return RESULT_STEP_CONTINUE;
                }
            }
        }

    }
    return RESULT_STEP_NOT_AVAILABLE;
}

int core_checkers::CommitStepKing(int x, int y, int toX, int toY)
{
    if (abs(x - toX) != abs(y - toY))	{
        return RESULT_STEP_NOT_AVAILABLE;
    }
    CheckerType type = gField[Cell(x, y)]->GetType();
    int killX = 0, killY = 0;
    int ability = AbilityMove(x, y);

    int chX = toX > x ? 1 : -1,
        chY = toY > y ? 1 : -1;

    //	проверяем доступность пути хода дамки
    for (int i = 1; x + i * chX != toX && y + i * chY != toY; i++)	{
        if (gField[Cell(x + i * chX, y + i * chY)] == nullptr)	{
            continue;
        }
        CheckerType t = gField[Cell(x + i * chX, y + i * chY)]->GetType();
        if (CompareTypes(t, type))	{
            return RESULT_STEP_NOT_AVAILABLE;	//	через своих прыгать нельзя
        }
        else	{	//	встретилась вражеская шашка
            if (killX != 0 && killY != 0)	{
                return RESULT_STEP_NOT_AVAILABLE;	//	нельзя есть сразу несколько шашек
            }
            if (gField[Cell(x + (i + 1) * chX, y + (i + 1) * chY)] == nullptr)	//	проверяем свободно ли следующее поле
            {
                killX = x + i * chX, killY = y + i * chY;
            }
        }
    }
    if (killX != 0 && killY != 0)	{
        gField[Cell(toX, toY)] = std::move(gField[Cell(x, y)]);
        DeleteChecker(Cell(killX, killY));
        if (AbilityMoveKing(toX, toY) == RESULT_STEP_ATTACK)	{
            return RESULT_STEP_CONTINUE;
        }
    }
    else if (ability == RESULT_STEP_ATTACK)	{	//	условие для того чтобы бить по возможность было обязательно
        return RESULT_STEP_MUST_ATTACK;
    }
    else {
        gField[Cell(toX, toY)] = std::move(gField[Cell(x, y)]);
    }
    return RESULT_STEP_COMPLETE;
}

int core_checkers::CompareTypes(int firstType, int secondType)
{
    if(firstType == C_BLACK || firstType == С_BLACK_KING)
    {
        if(secondType == C_BLACK || secondType == С_BLACK_KING)
        {
            return true;
        }
    }
    else if(firstType == C_WHITE || firstType == C_WHITE_KING)
    {
        if(secondType == C_WHITE || secondType == C_WHITE_KING)
        {
            return true;
        }
    }
    return false;
}

int core_checkers::AIStep()
{
    QTime time;
    time.start();
    for (; time.elapsed() < random(500, 2000);)
        qApp->processEvents();

    std::vector<int> AttackCheckers;
    std::vector<int> AvailableCheckers;
    int bX, bY, ability;
    //  формируем список доступных шашек
    for (auto i = gField.begin(); i != gField.end(); i++)
    {
        if (i->second == nullptr)	continue;
        if (CompareTypes(Step, i->second->GetType()))
        {
            Coord(i->first, bX, bY);
            ability = AbilityMove(bX, bY);
            if (ability == RESULT_STEP_ATTACK)	{
                AttackCheckers.push_back(i->first);
            }
            else if(ability == RESULT_STEP_AVAILABLE)   {
                AvailableCheckers.push_back(i->first);
            }
        }
    }
    std::vector<int> Steps;
    int toX, toY, rand;
    if(AttackCheckers.size())   {
        rand = random(0, (int)AttackCheckers.size() - 1);
        Coord(AttackCheckers[rand], bX, bY);
        AbilityMove(bX, bY, &Steps);
    }
    else if(AvailableCheckers.size())   {
        rand = random(0, (int)AvailableCheckers.size() - 1);
        Coord(AvailableCheckers[rand], bX, bY);
        AbilityMove(bX, bY, nullptr, &Steps);
    }
    else
    {
        return RESULT_STEP_NOT_AVAILABLE;
    }
    rand = random(0, (int)Steps.size() - 1);
    Coord(Steps[rand], toX, toY);
    return CommitStep(bX, bY, toX, toY);
}

int core_checkers::Cell(int x, int y)
{
    return y * FIELD_SIZE + x;
}

void core_checkers::Coord(int cell, int& x, int& y)	{
    x = cell % FIELD_SIZE; y = cell / FIELD_SIZE;
}
