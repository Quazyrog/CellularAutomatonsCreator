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


#ifndef COLORSETTINGSDIALOG_HPP
#define COLORSETTINGSDIALOG_HPP

#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QWidget>
#include <QColor>
#include <QPainter>

#include "CellInfo.hpp"



class MainWindow;


class ColorPreview : public QWidget
{
    Q_OBJECT

    QPainter *painter;
    QColor color;

    void paintEvent(QPaintEvent *e);

public:
    ColorPreview(QWidget *parent = 0);

    void setColor(int red, int green, int blue);
};


class ColorSettingsDialog : public QDialog
{
    Q_OBJECT

    MainWindow *mainWindow;

    ColorPreview *colorPreview;

    QSpinBox *status, *red, *green, *blue;
    QFormLayout *layout;

public:
    ColorSettingsDialog(MainWindow *parent);

public slots:
    void updateColorPreview();
    void updateRGB();

};

#endif // COLORSETTINGSDIALOG_HPP
