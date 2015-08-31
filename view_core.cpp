#include "checkers_controller.h"
#include "view_core.h"

NewGameWindow::NewGameWindow(QWidget* parent):
    QDialog(parent)
{
    setWindowTitle("Новая игра");

    QGridLayout *grid = new QGridLayout;
    //  Выбор черной стороны
    QGroupBox* groupBlack = new QGroupBox();

    QLabel* SideBlack = new QLabel("Черная сторона: ");
    radioSideBlack_Comp = new QRadioButton("Компьютер");
    radioSideBlack_Comp->setChecked(true);
    radioSideBlack_Peop = new QRadioButton("Человек");

    QHBoxLayout* BLayout = new QHBoxLayout();
    BLayout->addWidget(SideBlack);
    BLayout->addWidget(radioSideBlack_Comp);
    BLayout->addWidget(radioSideBlack_Peop);

    groupBlack->setLayout(BLayout);

    //  Выбор белой стороны
    QGroupBox* groupWhite = new QGroupBox();

    QLabel* SideWhite = new QLabel("Белая сторона: ");
    radioSideWhite_Comp = new QRadioButton("Компьютер");
    radioSideWhite_Peop = new QRadioButton("Человек");
    radioSideWhite_Peop->setChecked(true);

    QHBoxLayout* WLayout = new QHBoxLayout();
    WLayout->addWidget(SideWhite);
    WLayout->addWidget(radioSideWhite_Comp);
    WLayout->addWidget(radioSideWhite_Peop);

    groupWhite->setLayout(WLayout);

    QPushButton* confirm = new QPushButton("Начать игру", this);
    connect(confirm, SIGNAL(clicked(bool)), parent, SLOT(startNewGame(bool)));

    grid->addWidget(groupBlack, 0, 0);
    grid->addWidget(groupWhite, 1, 0);
    grid->addWidget(confirm, 2, 0);

    setLayout(grid);
}

Window::Window(Checkers* controller)
    : QMainWindow()
{
   this->controller = controller;

    dialog = new NewGameWindow(this);

    createFileDialog();
    createActions();
    createMenus();

    locationLabel = new QLabel(tr("Ход: - [0 белых, 0 черных]"));
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());
    this->statusBar()->addWidget(locationLabel);

    //  Поле
    field = new Field(this);
    setCentralWidget(field);

    //  Параметры
    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle(trUtf8("Шашки"));
    setFixedSize(800, 700);
}

void Window::setStatusBar(const QString &text)
{
    locationLabel->setText(text);
}

void Window::createFileDialog()
{
    fd_load = new QFileDialog(this, "Загрузка");
    fd_load->setFileMode(QFileDialog::ExistingFile);
    connect(fd_load, SIGNAL(fileSelected(const QString&)), this, SLOT(FileChosenLoad(const QString&)));

    fd_save = new QFileDialog(this, "Сохранение");
    fd_load->setFileMode(QFileDialog::AnyFile);
    connect(fd_save, SIGNAL(fileSelected(const QString&)), this, SLOT(FileChosenSave(const QString&)));
}

void Window::createActions()
{
    newGameAction = new QAction(tr("&Новая игра"), this);
    newGameAction->setStatusTip(tr("Начать новую игру"));
    newGameAction->setShortcut(QKeySequence::New);
    connect(newGameAction, SIGNAL(triggered()), dialog, SLOT(show()));

    openAction = new QAction(tr("&Загрузить игру ..."), this);
    openAction->setStatusTip(tr("Загрузить игру из файла"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), fd_load, SLOT(show()));

    saveGameAction = new QAction(tr("Сохранить"), this);
    saveGameAction->setStatusTip(tr("Сохранить игру в файл"));
    saveGameAction->setShortcut(QKeySequence::Save);
    connect(saveGameAction, SIGNAL(triggered()), this, SLOT(saveGame()));

    exitAction = new QAction(tr("&Выйти"), this);
    exitAction->setStatusTip(tr("Выйти из игры"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void Window::createMenus()
{
    fileMenu = this->menuBar()->addMenu(tr("Игра"));
    fileMenu->addAction(newGameAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveGameAction);
    fileMenu->addAction(exitAction);
}

void Window::showWarning(const char* warning)
{
    QMessageBox::information(0, "Предупреждение", warning);
}

void Window::startNewGame(bool)
{
    dialog->close();

    controller->NewGame(dialog->radioSideWhite_Comp->isChecked(), dialog->radioSideBlack_Comp->isChecked());
    controller->UpdateField();
}

void Window::FileChosenLoad(const QString& filename)
{
    controller->LoadGame(filename.toStdString().c_str());
}

void Window::saveGame()
{
    if(controller->isStartGame())   {
        fd_save->show();
    }
    else    {
        showWarning("Игра еще не начата, сохранять нечего");
    }
}

void Window::FileChosenSave(const QString& filename)
{
    controller->SaveGame(filename.toStdString().c_str());
}

void Window::SelectedCell(int x, int y)
{
    controller->SelectedCell(x, y);
}
