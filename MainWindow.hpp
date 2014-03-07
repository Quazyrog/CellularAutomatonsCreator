#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QAction>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QResource>
#include <fstream>


class GridSettingsWindow;
class PopulationViewWidget;


class MainWindow : public QMainWindow
{
    friend class GridSettingsWindow;

    Q_OBJECT
private:
    GridSettingsWindow *gridSettingsWindow;
    QPlainTextEdit *licenseBrowser;

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
    QAction *actionAboutQt, *actionShowLicense;
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
    void showLicense();

};

#endif // MAINWINDOW_HPP
