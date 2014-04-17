/* Copyright 2014 Wojciech Matusiak
 *
 * This file is part of CellularAutomatonCreator.
 *
 * CellularAutomatonCreator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CellularAutomatonCreator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CellularAutomatonCreator.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "MainWindow.hpp"
#include "PopulationViewWidget.hpp"
#include "GridSettingsWindow.hpp"
#include "AutomatonScriptEditor.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    statusFillColors[0] = Qt::white;
    statusFillColors[1] = Qt::black;
    statusFillColors[2] = Qt::blue;
    statusFillColors[3] = Qt::green;
    statusFillColors[4] = Qt::red;
    statusFillColors[5] = Qt::yellow;

    gridSettingsWindow = new GridSettingsWindow(this);
    automatonEditor = new AutomatonScriptEditor(this);
    colorSettings = new ColorSettingsDialog(this);

    generation = 0;
    world = new CellularAutomaton();
    nextGeneration();
    viewer = new PopulationViewWidget(this, world);
    setCentralWidget(viewer);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), viewer, SLOT(nextGeneration()));
    connect(timer, SIGNAL(timeout()), this, SLOT(nextGeneration()));

    setWindowTitle(tr("Automaty Komórkowe"));
    setWindowIcon(QIcon((QString) ":/icons/MainWindow.svg"));
    setMinimumSize(QSize(600, 400));

    createFileMenu();
    createSimulationMenu();
    createGridMenu();
    createAutomatonMenu();
    createHelpMenu();

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

    actionColorSettings = new QAction(tr("Konfiguruj kolory"), this);
    connect(actionColorSettings, SIGNAL(triggered()), colorSettings, SLOT(show()));
    simulationMenu->addAction(actionColorSettings);
}


void MainWindow::createFileMenu()
{
    fileMenu = new QMenu(this);
    fileMenu->setTitle(tr("&Plik"));
    menuBar()->addMenu(fileMenu);

    actionFork = new QAction(tr("Nowe okno"), this);
    actionFork->setShortcut(QKeySequence::New);
    connect(actionFork, SIGNAL(triggered()), this, SLOT(fork()));
    fileMenu->addAction(actionFork);

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

    actionReset = new QAction(this);
    actionReset->setText(tr("Wyczyść siatkę"));
    actionReset->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(actionReset, SIGNAL(triggered()), viewer, SLOT(resetGrid()));
    connect(actionReset, SIGNAL(triggered()), this, SLOT(newGame()));
    gridMenu->addAction(actionReset);

    actionGridRND = new QAction(this);
    actionGridRND->setText(tr("Wygeneruj losową siatkę"));
    connect(actionGridRND, SIGNAL(triggered()), this, SLOT(newGame()));
    gridMenu->addAction(actionGridRND);

    gridMenu->addSeparator();

    actionChangeBrush = new QAction(tr("Zmiań pedzel"), this);
    actionChangeBrush->setShortcut(Qt::Key_F2);
    connect(actionChangeBrush, SIGNAL(triggered()), timer, SLOT(stop()));
    connect(actionChangeBrush, SIGNAL(triggered()), this, SLOT(changeBrush()));
    gridMenu->addAction(actionChangeBrush);

    gridMenu->addSeparator();

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
    connect(actionScriptSettings, SIGNAL(triggered()), timer, SLOT(stop()));
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


void MainWindow::setStatusFillColor(StatusT status, QColor color)
{
    statusFillColors[status] = color;
    viewer->update();
}


void MainWindow::changeBrush()
{
    bool ok;
    int val = QInputDialog::getInt(this, tr("Zmana pędzla"), tr("Podaj nowy pędzel statusu"), viewer->getBrush(), 0, STATUS_NUMBER - 1,
                                   1, &ok);
    if (ok) {
        viewer->setBrush(val);
        repaint();
    }
}


void MainWindow::fork()
{
    QProcess newProcess;
    if (!newProcess.startDetached(QFileInfo(QCoreApplication::applicationFilePath()).fileName())) {
        QMessageBox err(this);
        err.setIcon(QMessageBox::Critical);
        err.setWindowTitle(tr("Błąd"));
        err.setText(tr("Nie udało się utworzyć procesu."));
        err.exec();
    }
}

