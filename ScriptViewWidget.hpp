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


#ifndef SCRIPTVIEWWIDGET_HPP
#define SCRIPTVIEWWIDGET_HPP

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>
#include <QFont>
#include <QInputDialog>

#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"
#include "BrickEditDialog.hpp"


typedef ScriptBrickIf::ComparisionOperators ComparisionOperator;


class MainWindow;


class ScriptViewWidget : public QWidget
{
    Q_OBJECT

    struct Brick {
        QString left, right;
        ComparisionOperator op;
        StatusT status;

        int column;

        Brick *under, *after, *parent;
        Brick *prev, *next;

        ~Brick();

        Brick *deepest();
    };

    Brick *createBrick(QString left, ComparisionOperator op, QString right, StatusT color);
    void addBrickToList(Brick *after, Brick *toInsert);
    QString getBrickText(Brick *b);
    QColor getBrickColor(Brick *b);

    int rows, columns, selectionRow;

    int brickWidth, brickHeight;
    int textSize;
    int horizontalMargin, verticalMargin;
    int horizontalSpacing, verticalSpacing;

    StatusT defaultStatus;

    QPainter *painter;
    QFont textFont;

    Brick *firstBrick, *selected;

    MainWindow *mainWindow;

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void mkSize(bool recountRows = false);

    ScriptBrick *compileBrick(Brick *brick);

public:
    explicit ScriptViewWidget(MainWindow *mainWindow, QWidget *parent = 0);
    ~ScriptViewWidget();

    int getBrickWidth();
    void setBrickWidth(int value);

    int getBrickHeight();
    void setBrickHeight(int value);

    int getTextSize();
    void setTextSize(int value);

    int getVerticalMargin();
    void setVerticalMargin(int value);

    int getHorizontalMargin();
    void setHorizontalMargin(int value);

    int getHorizontalSpacing();
    void setHorizontalSpacing(int value);

    int getVerticalSpacing();
    void setVerticalSpacing(int value);

    ScriptBrick *compile();

public slots:
    void eraseSelected();
    void editSelected();
    void addUnderSelected();
    void addAfterSelected();
    void editDefault();

};

#endif // SCRIPTVIEWWIDGET_HPP
