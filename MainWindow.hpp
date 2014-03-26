#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QAction>

#include "CellInfo.hpp"


class GridSettingsWindow;
class PopulationViewWidget;
class AutomatonScriptEditor;


class MainWindow : public QMainWindow
{
    friend class GridSettingsWindow;

    Q_OBJECT
private:
    QColor statusFillColors[STATUS_NUMBER], statusTextColors[STATUS_NUMBER];

    GridSettingsWindow *gridSettingsWindow;
    AutomatonScriptEditor *automatonEditor;

    QTimer *timer;
    PopulationViewWidget *theWorld;
    int generation;

    QMenu *simulationMenu;
    QAction *actionPause, *actionReset, *actionGridRND;
    void createSimulationMenu();

    QMenu *fileMenu;
    QAction *actionQuit;
    void createFileMenu();

    QMenu *helpMenu;
    QAction *actionAboutQt;
    void createHelpMenu();

    QMenu *gridMenu;
    QAction *actionGridSettings;
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

    QColor getStatusTextColor(StatusT status) const;

private slots:
    void nextGeneration();
    void pause();
    void newGame();

};

#endif // MAINWINDOW_HPP
