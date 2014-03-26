#include "MainWindow.hpp"
#include "PopulationViewWidget.hpp"
#include "GridSettingsWindow.hpp"
#include "AutomatonScriptEditor.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    statusFillColors[0] = Qt::black;
    statusTextColors[0] = Qt::white;
    statusFillColors[1] = Qt::white;
    statusTextColors[1] = Qt::black;
    statusFillColors[2] = Qt::blue;
    statusTextColors[2] = Qt::white;
    statusFillColors[3] = Qt::green;
    statusTextColors[3] = Qt::white;
    statusFillColors[4] = Qt::red;
    statusTextColors[4] = Qt::black;
    statusFillColors[5] = Qt::yellow;
    statusTextColors[5] = Qt::black;

    generation = 0;
    nextGeneration();
    theWorld = new PopulationViewWidget(this, 40, 20);
    setCentralWidget(theWorld);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), theWorld, SLOT(nextGeneration()));
    connect(timer, SIGNAL(timeout()), this, SLOT(nextGeneration()));

    gridSettingsWindow = new GridSettingsWindow(this);
    automatonEditor = new AutomatonScriptEditor(this);

    createFileMenu();
    createSimulationMenu();
    createGridMenu();
    createAutomatonMenu();
    createHelpMenu();

    setWindowTitle(tr("Automaty Komórkowe"));
    setWindowIcon(QIcon((QString) ":/icons/MainWindow.svg"));
    setMinimumSize(QSize(600, 400));

    showMaximized();
}

MainWindow::~MainWindow()
{

}


void MainWindow::nextGeneration()
{
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation++));
}


void MainWindow::pause()
{
    if (timer->isActive())
        timer->stop();
    else
        timer->start();
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation) + QString(" (wstrzymane)"));
}


void MainWindow::createSimulationMenu()
{
    simulationMenu = new QMenu(this);
    simulationMenu->setTitle(tr("&Symulacja"));
    menuBar()->addMenu(simulationMenu);

    actionPause = new QAction(this);
    actionPause->setText(tr("Wstrzymaj / Wznów"));
    actionPause->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
    simulationMenu->addAction(actionPause);

    actionReset = new QAction(this);
    actionReset->setText(tr("Wyczyść siatkę"));
    actionReset->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(actionReset, SIGNAL(triggered()), theWorld, SLOT(resetGrid()));
    connect(actionReset, SIGNAL(triggered()), this, SLOT(newGame()));
    simulationMenu->addAction(actionReset);

    actionGridRND = new QAction(this);
    actionGridRND->setText(tr("Wygeneruj losową siatkę"));
    connect(actionGridRND, SIGNAL(triggered()), theWorld, SLOT(makeRandomGrid()));
    connect(actionGridRND, SIGNAL(triggered()), this, SLOT(newGame()));
    simulationMenu->addAction(actionGridRND);
}


void MainWindow::createFileMenu()
{
    fileMenu = new QMenu(this);
    fileMenu->setTitle(tr("&Plik"));
    menuBar()->addMenu(fileMenu);

    actionQuit = new QAction(this);
    actionQuit->setText(tr("Zakończ"));
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    fileMenu->addSeparator();
    fileMenu->addAction(actionQuit);
}


void MainWindow::createHelpMenu()
{
    helpMenu = new QMenu(this);
    helpMenu->setTitle(tr("&Pomoc"));
    menuBar()->addMenu(helpMenu);

    actionAboutQt = new QAction(this);
    actionAboutQt->setText(tr("O Qt"));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    helpMenu->addAction(actionAboutQt);
}


void MainWindow::createGridMenu()
{
    gridMenu = new QMenu(this);
    gridMenu->setTitle(tr("S&iatka"));
    menuBar()->addMenu(gridMenu);

    actionGridSettings = new QAction(tr("Konfiguruj rozmiar siatki"), this);
    connect(actionGridSettings, SIGNAL(triggered()), gridSettingsWindow, SLOT(show()));
    gridMenu->addAction(actionGridSettings);
}


void MainWindow::createAutomatonMenu()
{
    automatonMenu = new QMenu(this);
    automatonMenu->setTitle(tr("&Automat"));
    menuBar()->addMenu(automatonMenu);

    actionScriptSettings = new QAction(tr("Konfiguruj reguły przejścia"), this);
    connect(actionScriptSettings, SIGNAL(triggered()), automatonEditor, SLOT(show()));
    automatonMenu->addAction(actionScriptSettings);
}


void MainWindow::newGame()
{
    generation = 0;
    timer->stop();
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation) + QString(" (wstrzymane)"));
}


void MainWindow::setGenerationInterval(int msec)
{
    timer->setInterval(msec);
}


int MainWindow::getGenerationInterval()
{
    return timer->interval();
}


QColor MainWindow::getStatusFillColor(StatusT status) const
{
    return statusFillColors[status];
}


QColor MainWindow::getStatusTextColor(StatusT status) const
{
    return statusTextColors[status];
}
