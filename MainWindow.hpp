/* Copyright 2014 Wojciech Matusiak
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

class GridViewer;
class CellularAutomaton;
class StateSwitchDock;


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QStatusBar *_statusBar;

    void buildMenuBar();
    QMenuBar *_menuBar;
    QMenu *_fileMenu;

    GridViewer *_gridViewer;
    CellularAutomaton *_automaton;
    StateSwitchDock *_stateSwitch;

public:
    explicit MainWindow(QWidget *parent = 0);

    virtual ~MainWindow();

};

#endif // MAINWINDOW_HPP
