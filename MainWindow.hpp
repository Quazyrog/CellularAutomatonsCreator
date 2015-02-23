/* Copyright 2014, 2015 Wojciech Matusiak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui>
#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QSettings>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QFileDialog>
#include <QMessageBox>

class GridViewer;
class StateSwitchDock;
namespace Scripting {
    class CellularAutomaton;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QStatusBar *_statusBar;

    void createMenuBar();
    QMenu *_fileMenu;
    QAction *_fileNewAction;
    QAction *_fileOpenAction;
    QAction *_fileSaveAction;
    QAction *_fileSaveAsAction;
    QAction *_fileCloseAction;

    GridViewer *_gridViewer;
    Scripting::CellularAutomaton *_automaton;
    StateSwitchDock *_stateSwitch;

    void closeEvent(QCloseEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = 0);

    virtual ~MainWindow();

public slots:
    void openFile();
};

#endif // MAINWINDOW_HPP
