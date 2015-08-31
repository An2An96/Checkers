#include "view_field.h"

Field::Field(QWidget *parent):
    QWidget(parent)
{
    this->parent = parent;
    CreateField();
    drawField();
    ClearField();
}

bool Field::SetCheckerCell(int x, int y, int type)
{
    if(x < 0 || y < 0 || x >= FIELD_SIZE || y >= FIELD_SIZE)    {
        return false;
    }
    return field[y][x]->setCheckerType(type);
}

void Field::CreateField()
{
    for(int j = 0; j < FIELD_SIZE; j++)  {
        QVector<Cell*> temp;
        for(int i = 0; i < FIELD_SIZE; i++)  {
            if((j + i % 2) % 2 )    {
                temp.push_back(new Cell(WHITE));
            }   else    {
                temp.push_back(new Cell(BLACK, C_NONE, i, j));
                connect(temp.back(), SIGNAL(SelectedCell(int, int)), parent, SLOT(SelectedCell(int, int)));
            }
        }
        field.push_back(temp);
    }
}

void Field::drawField()
{
    QGridLayout* layout = new QGridLayout;
    for(int j = 0; j < FIELD_SIZE; j++)   {
        for(int i = 0; i < FIELD_SIZE; i++)   {
            layout->addWidget(field.at(j).at(i), j, i, 2, 4);
        }
    }
    this->setLayout(layout);
}

void Field::SetCellSelect(int x, int y, bool s)
{
    field[y][x]->setSelect(s);
}

void Field::ClearField()
{
    for(int i = 0; i < FIELD_SIZE; i++){
        for(int j = 0; j < FIELD_SIZE; j++){
            field[i][j]->setCheckerType(C_NONE);
            field[i][j]->setSelect(false);
        }
    }
}
