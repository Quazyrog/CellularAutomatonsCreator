#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QAction>


class GridSettingsWindow;
class PopulationViewWidget;


class MainWindow : public QMainWindow
{
    friend class GridSettingsWindow;

    Q_OBJECT
private:
    GridSettingsWindow *gridSettingsWindow;

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

    QMenu *settingsMenu;
    QAction *actionGridSettings;
    void createSettingsMenu();


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getGenerationInterval();
    void setGenerationInterval(int msec);

private slots:
    void nextGeneration();
    void pause();
    void newGame();
    void showGridSettingsWindow();

};

#endif // MAINWINDOW_HPP
