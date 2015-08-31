#pragma once

#include <QGridLayout>

#include "checkers_data.h"
#include "view_cell.h"

class Field : public QWidget
{
    Q_OBJECT

public:
    explicit Field(QWidget* = 0);

    bool SetCheckerCell(int, int, int);
    void SetCellSelect(int, int, bool);
    void ClearField();

private:
    void CreateField();
    void drawField();

    QVector< QVector<Cell*> > field;
    QWidget *parent;
};
