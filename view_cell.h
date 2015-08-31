#pragma once

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>

#include "checkers_data.h"

class Cell : public QWidget
{
    Q_OBJECT

public:
    Cell(const int color);
    Cell(const int color, const int checkerType, const int x, const int y);

    bool setCheckerType(int type);
    void setSelect(const bool s);

signals:
    void SelectedCell(int, int);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *me);

private:
    int X, Y;
    int CheckerType = C_NONE, CellColor;
    bool isSelect = false;

    void drawCell();
    void drawChecker();

public:
    static const int CellSize = 70;
    static const int CheckerSize = 30;
};
