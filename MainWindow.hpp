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


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QAction>
#include <QInputDialog>
#include <QProcess>
#include <QFileInfo>

#include "CellInfo.hpp"
#include "CellularAutomaton.hpp"
#include "AutomatonScriptEditor.hpp"
#include "ColorSettingsDialog.hpp"

class GridSettingsWindow;
class PopulationViewWidget;
class AutomatonScriptEditor;


class MainWindow : public QMainWindow
{
    friend class GridSettingsWindow;
    friend class AutomatonScriptEditor;

    Q_OBJECT
private:
    QColor statusFillColors[STATUS_NUMBER];

    GridSettingsWindow *gridSettingsWindow;
    AutomatonScriptEditor *automatonEditor;
    ColorSettingsDialog *colorSettings;

    QTimer *timer;
    PopulationViewWidget *viewer;
    CellularAutomaton *world;
    int generation;

    QMenu *simulationMenu;
    QAction *actionPause,  *actionColorSettings;
    void createSimulationMenu();

    QMenu *fileMenu;
    QAction *actionQuit, *actionFork;
    void createFileMenu();

    QMenu *helpMenu;
    QAction *actionAboutQt;
    void createHelpMenu();

    QMenu *gridMenu;
    QAction *actionGridSettings,  *actionReset, *actionGridRND, *actionChangeBrush;
    void createGridMenu();

    QMenu *automatonMenu;
    QAction *actionScriptSettings;
    void createAutomatonMenu();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getGenerationInterval();
    void setGenerationInterval(int msec);

    QColor getStatusFillColor(StatusT status) const;
    void setStatusFillColor(StatusT status, QColor color);

public slots:
    void nextGeneration();
    void pause();
    void newGame();
    void changeBrush();
    void fork();

signals:
    void newGeneration();
    void stopSimulation();

};

#endif // MAINWINDOW_HPP
