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


#include "PopulationViewWidget.hpp"
#include "MainWindow.hpp"



PopulationViewWidget::PopulationViewWidget(MainWindow *parent, CellularAutomaton *automaton):
    QWidget(parent)
{
    painter = new QPainter(this);

    normalPen.setWidth(1);
    normalPen.setColor(Qt::black);
    highlightedPen.setWidth(2);
    highlightedPen.setColor(Qt::yellow);

    setMouseTracking(true);
    cursorCellX = cursorCellY = -1;
    statusBrush = 1;

    gridSize.setWidth(40);
    gridSize.setHeight(20);
    world = automaton;
    world->createGrid(gridSize.width(), gridSize.height());

    mainWindow = parent;
}


PopulationViewWidget::~PopulationViewWidget() {

}


void PopulationViewWidget::paintEvent(QPaintEvent *e)
{
    painter->begin(this);

    QRect updateArea;
    updateArea.setX((e->rect().x() - hmargin) / cellSize);
    updateArea.setX((updateArea.x() < 0 ? 0 : updateArea.x()));
    updateArea.setY((e->rect().y() - vmargin) / cellSize);
    updateArea.setY((updateArea.y() < 0 ? 0 : updateArea.y()));
    updateArea.setWidth(e->rect().width() / cellSize);
    updateArea.setHeight(e->rect().height() / cellSize);

    int xlim = updateArea.x() + updateArea.width(), ylim = updateArea.y() + updateArea.height();
    xlim = (xlim > gridSize.width() ? gridSize.width() : xlim);
    ylim = (ylim > gridSize.height() ? gridSize.height() : ylim);

    for (int x = updateArea.x(); x < xlim; x++) {
        for (int y = updateArea.y(); y < ylim; y++) {
            painter->setBrush(mainWindow->getStatusFillColor(world->get(x, y)));
            painter->drawRect(hmargin + x * cellSize, vmargin + y * cellSize, cellSize, cellSize);
        }
    }

    if (0 <= cursorCellX && cursorCellX < gridSize.width() && 0 <= cursorCellY && cursorCellY < gridSize.height()) {
        QColor c1(mainWindow->getStatusFillColor(world->get(cursorCellX, cursorCellY))), c2(mainWindow->getStatusFillColor(statusBrush)), cmix;
        cmix.setRed((c1.red() + c2.red()) / 2);
        cmix.setGreen((c1.green() + c2.green()) / 2);
        cmix.setBlue((c1.blue() + c2.blue()) / 2);

        painter->setPen(highlightedPen);
        painter->setBrush(QBrush(cmix));
        painter->drawRect(hmargin + cursorCellX * cellSize, vmargin + cursorCellY * cellSize, cellSize, cellSize);
    }

    painter->end();
}


void PopulationViewWidget::nextGeneration()
{
    world->nextGeneration();

    update(0, 0, width(), height());
}


void PopulationViewWidget::mouseMoveEvent(QMouseEvent *e)
{
    int oldx = cursorCellX ;
    cursorCellX = (e->x() - hmargin) / cellSize;
    int oldy = cursorCellY;
    cursorCellY = (e->y() - vmargin) / cellSize;

    if (oldx != cursorCellX || oldy != cursorCellY) {
        update((oldx - 1) * cellSize + hmargin, (oldy - 1) * cellSize + vmargin, cellSize * 3, cellSize * 3);
    }
}


void PopulationViewWidget::resizeEvent(QResizeEvent * e)
{
    countDrawParams();
}

void PopulationViewWidget::mousePressEvent(QMouseEvent *e)
{
    if (cursorCellX < 0 || cursorCellX >= gridSize.width() || cursorCellY < 0 || cursorCellY >= gridSize.height())
        return;
    if (e->button() != Qt::LeftButton)
        return;
    world->set(cursorCellX, cursorCellY, statusBrush);
    update((cursorCellX - 1) * cellSize + hmargin, (cursorCellY - 1) * cellSize + vmargin, cellSize *3, cellSize * 3);
}


void PopulationViewWidget::resetGrid()
{
    world->clear();
    update(0, 0, width(), height());
}


void PopulationViewWidget::setBrush(StatusT brush)
{
    if (STATUS_NUMBER <= brush)
        return;
    statusBrush = brush;
}


void PopulationViewWidget::resizeGrid(size_t width, size_t height)
{
    world->createGrid(width, height);

    gridSize.setWidth(width);
    gridSize.setHeight(height);

    countDrawParams();
    update(0, 0, this->width(), this->height());
}


QSize PopulationViewWidget::getGridSize()
{
    return gridSize;
}


void PopulationViewWidget::countDrawParams()
{
    cellSize = std::min((width() - 2 * minMargin) / gridSize.width(), (height() - 2 * minMargin) / gridSize.height());
    hmargin = (width() - cellSize * gridSize.width()) / 2;
    vmargin = (height() - cellSize * gridSize.height()) / 2;
}


StatusT PopulationViewWidget::getBrush()
{
    return statusBrush;
}
