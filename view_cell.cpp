#include "view_cell.h"

Cell::Cell(int color)
{
    CellColor = color;
    setFixedSize(CellSize, CellSize);
}

Cell::Cell(int color, int checkerType, int x, int y)
    : CellColor(color), CheckerType(checkerType), X(x), Y(y)
{
    setFixedSize(CellSize, CellSize);
}

void Cell::setSelect(bool s)
{
    isSelect = s;
}

bool Cell::setCheckerType(int type)
{
    if(CellColor == BLACK && type >= C_BLACK && type <= C_NONE)  {
        CheckerType = type;
        return true;
    }
    return false;
}

void Cell::paintEvent(QPaintEvent *)
{
    if(isSelect)    {
        QPainter painter(this);
        painter.setBrush(QColor(120,65,15));
        painter.drawRect(0, 0, width(), height());
        drawChecker();
    }
    else {
        drawCell();
    }
    update();
}

void Cell::mousePressEvent(QMouseEvent *me)
{
    if(me->buttons() && Qt::LeftButton) {
        if(CellColor == BLACK){
            emit SelectedCell(X, Y);
        }
    }
}

void Cell::drawCell()
{
    QPainter painter(this);
    if(CellColor == BLACK)  {
        painter.setBrush(QColor(0xbc,0x5c,0xc));
    }
    else if(CellColor == WHITE) {
        painter.setBrush(Qt::white);
    }
    else    {
        return;
    }
    painter.drawRect(0, 0, width(), height());
    drawChecker();
    update();
}

void Cell::drawChecker()
{
    QPainter painter(this);
    switch (CheckerType)    {
    case C_BLACK: {
        painter.setBrush(QColor(210,70,65));
        QPen pen(QColor(150,40,35));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize,CheckerSize);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize/2,CheckerSize/2);
        break;
    }
    case C_WHITE: {
        painter.setBrush(QColor(220,220,220));
        QPen pen(QColor(110,110,110));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize,CheckerSize);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize/2,CheckerSize/2);
        break;
    }
    case С_BLACK_KING:    {
        painter.setBrush(QColor(210,70,65));
        QPen pen(QColor(150,40,35));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize,CheckerSize);
        painter.setBrush(QColor(220, 200, 90));
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize/2,CheckerSize/2);
        break;
    }
    case C_WHITE_KING:    {
        painter.setBrush(QColor(220,220,220));
        QPen pen(QColor(110,110,110));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize,CheckerSize);
        painter.setBrush(QColor(220, 200, 90));
        painter.drawEllipse(QPoint(width()/2,height()/2),CheckerSize/2,CheckerSize/2);
        break;
    }
    default:
        return;
    }
    update();
}
