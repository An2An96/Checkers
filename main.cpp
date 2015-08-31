#include <QApplication>
#include <time.h>

#include "checkers_controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand((int)time(NULL));

    Checkers game;
    return a.exec();
}
