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


#ifndef POPULATIONVIEWWIDGET_HPP
#define POPULATIONVIEWWIDGET_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QSize>

#include <iostream>

class MainWindow;


#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"


class PopulationViewWidget : public QWidget
{
    Q_OBJECT
private:
    MainWindow *mainWindow;

    QPainter *painter;
    QPen normalPen,  highlightedPen;
    QColor colors[5];
    int cursorCellX, cursorCellY;
    void countDrawParams();

    QRect gridArea;
    int cellSize, hmargin, vmargin;
    QSize gridSize;
    CellularAutomaton *world;
    const int minMargin = 5;

    StatusT statusBrush;

    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *e);

public:
    explicit PopulationViewWidget(MainWindow *parent, CellularAutomaton *automaton);
    virtual ~PopulationViewWidget();

    void resizeGrid(size_t width, size_t height);
    QSize getGridSize();

    StatusT getBrush();

public slots:
    void nextGeneration();
    void resetGrid();
    void setBrush(StatusT brush);

};

#endif // POPULATIONVIEWWIDGET_HPP
