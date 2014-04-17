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


#ifndef GRIDSETTINGDWINDOW_HPP
#define GRIDSETTINGDWINDOW_HPP

#include <QMainWindow>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QFormLayout>
#include <iostream>

class MainWindow;



class GridSettingsWindow : public QDialog
{
    Q_OBJECT
private:
    MainWindow *parent;

    QGridLayout *form;

    QSpinBox *widthBox, *heightBox;
    QGroupBox *sizeForm;
    QFormLayout *sizeFormLayout;
    void createSizeSettings(int row);

    QPushButton *applyButton, *cancelButton;
    void createButtons(int row);

    void showEvent(QShowEvent *e);

public:
    GridSettingsWindow(MainWindow *parent);

public slots:
    void applySettings();
    void readSettings();

};

#endif // GRIDSETTINGDWINDOW_HPP
