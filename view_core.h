#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QAction>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QStatusBar>

#include "view_field.h"

class Checkers;

class NewGameWindow:
        public QDialog
{
    Q_OBJECT

public:
    NewGameWindow(QWidget* parent);

    QRadioButton *radioSideBlack_Comp, *radioSideBlack_Peop, *radioSideWhite_Comp, *radioSideWhite_Peop;
};

class Window:
        public QMainWindow
{
    Q_OBJECT

public:
    Window(Checkers*);

    void showWarning(const char*);
    void setStatusBar(const QString&);

    Field* field;

public slots:
    void startNewGame(bool);

private slots:
    void saveGame();
    void FileChosenLoad(const QString&);
    void FileChosenSave(const QString&);
    void SelectedCell(int, int);

private:
    void createFileDialog();
    void createActions();
    void createMenus();

    QMenu *fileMenu, *optionsMenu, *editMenu;
    QAction *newGameAction, *openAction, *saveGameAction, *exitAction;
    QLabel *locationLabel;
    QFileDialog *fd_load, *fd_save;

    NewGameWindow* dialog;
    Checkers* controller;
};
