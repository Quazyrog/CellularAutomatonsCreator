#include "MainWindow.hpp"
#include "PopulationViewWidget.hpp"
#include "GridSettingsWindow.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    generation = 0;
    nextGeneration();
    theWorld = new PopulationViewWidget(this, 40, 20);
    setCentralWidget(theWorld);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), theWorld, SLOT(nextGeneration()));
    connect(timer, SIGNAL(timeout()), this, SLOT(nextGeneration()));

    createFileMenu();
    createSimulationMenu();
    createSettingsMenu();
    createHelpMenu();

    setWindowTitle(tr("Automaty Komórkowe"));
    setWindowIcon(QIcon((QString) ":/windows/MainWindow.png"));
    setMinimumSize(QSize(600, 400));

    gridSettingsWindow = new GridSettingsWindow(this);

    licenseBrowser = new QPlainTextEdit();
    licenseBrowser->setReadOnly(true);
    licenseBrowser->resize(600, 400);

    showMaximized();
}

MainWindow::~MainWindow()
{

}


void MainWindow::nextGeneration()
{
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation++));
}


void MainWindow::createSimulationMenu()
{
    simulationMenu = new QMenu(this);
    simulationMenu->setTitle(tr("&Symulacja"));
    menuBar()->addMenu(simulationMenu);

    actionPause = new QAction(this);
    actionPause->setText(tr("Wstrzymaj / Wznów"));
    actionPause->setShortcut(Qt::Key_F5);
    connect(actionPause, SIGNAL(triggered()), this, SLOT(pause()));
    simulationMenu->addAction(actionPause);

    actionReset = new QAction(this);
    actionReset->setText(tr("Wyczyść siatkę"));
    connect(actionReset, SIGNAL(triggered()), theWorld, SLOT(resetGrid()));
    connect(actionReset, SIGNAL(triggered()), this, SLOT(newGame()));
    simulationMenu->addAction(actionReset);

    actionGridRND = new QAction(this);
    actionGridRND->setText(tr("Wygeneruj losową siatkę"));
    connect(actionGridRND, SIGNAL(triggered()), theWorld, SLOT(makeRandomGrid()));
    connect(actionGridRND, SIGNAL(triggered()), this, SLOT(newGame()));
    simulationMenu->addAction(actionGridRND);
}


void MainWindow::pause()
{
    if (timer->isActive())
        timer->stop();
    else
        timer->start();
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation) + QString(" (wstrzymane)"));
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

    actionShowLicense = new QAction(tr("Licencja"), this);
    connect(actionShowLicense, SIGNAL(triggered()), this, SLOT(showLicense()));
    helpMenu->addAction(actionShowLicense);
}


void MainWindow::createSettingsMenu()
{
    settingsMenu = new QMenu(this);
    settingsMenu->setTitle(tr("&Konfiguracja"));
    menuBar()->addMenu(settingsMenu);

    actionGridSettings = new QAction(tr("Konfiguruj rozmiar siatki"), this);
    connect(actionGridSettings, SIGNAL(triggered()), this, SLOT(showGridSettingsWindow()));
    settingsMenu->addAction(actionGridSettings);
}


void MainWindow::newGame()
{
    generation = 0;
    timer->stop();
    statusBar()->showMessage(tr("Polkolenie ") + QString::number(generation) + QString(" (wstrzymane)"));
}


void MainWindow::showGridSettingsWindow()
{
    gridSettingsWindow->show();
}


void MainWindow::showLicense()
{
    bool textLoaded = false;
    if (!textLoaded) {
        licenseBrowser->appendPlainText((char*) QResource(":/license/gpl-3.0.txt").data());
    }
    licenseBrowser->show();
}


void MainWindow::setGenerationInterval(int msec)
{
    timer->setInterval(msec);
}


int MainWindow::getGenerationInterval()
{
    return timer->interval();
}
