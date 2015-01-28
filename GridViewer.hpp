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

#ifndef GRIDVIEWER_HPP
#define GRIDVIEWER_HPP

#include <QtGui>
#include <QWidget>

#include "Exception.hpp"

class CellularAutomaton;


class GridViewer : public QWidget
{
private:
    CellularAutomaton *_automaton;

    QPainter *_painter;

    int _activeCellX;
    int _activeCellY;
    quint16 _stateBrush;


protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

public:
    GridViewer(QWidget *parent);
    virtual ~GridViewer();

    inline int getCellSize() const;
    inline int getLeftMargin() const;
    inline int getTopMargin() const;

    void setDisplayedAutomaton(CellularAutomaton *automaton) throw (Exceptions::NullPointerException);

    quint16 getStateBrush() const;
    void setStateBrush(quint16 brush) throw (Exceptions::IllegalArgumentException);

};

#endif // GRIDVIEWER_HPP
