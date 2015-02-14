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

#include "MainWindow.hpp"

#include "GridViewer.hpp"
#include "CellularAutomaton.hpp"
#include "StateSwitchWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _automaton = new Scripting::CellularAutomaton(4, 40, 30);

    _gridViewer = new GridViewer(this);
    _gridViewer->setDisplayedAutomaton(_automaton);
    setCentralWidget(_gridViewer);

    _stateSwitch = new StateSwitchDock(_automaton, _gridViewer, this);
    addDockWidget(Qt::LeftDockWidgetArea, _stateSwitch);

    _statusBar = new QStatusBar(this);
    setStatusBar(_statusBar);

    buildMenuBar();

    QSettings settings(".cacsettings", QSettings::NativeFormat);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    setStatusTip("Done");
}


void MainWindow::buildMenuBar()
{
    _menuBar = new QMenuBar(this);
    setMenuBar(_menuBar);

    _fileMenu = new QMenu("File", this);

    _menuBar->addMenu(_fileMenu);
}


MainWindow::~MainWindow()
{
    delete _automaton;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(".cacsettings", QSettings::NativeFormat);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
}
