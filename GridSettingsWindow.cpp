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


#include "GridSettingsWindow.hpp"
#include "PopulationViewWidget.hpp"
#include "MainWindow.hpp"



GridSettingsWindow::GridSettingsWindow(MainWindow *parent) :
    QDialog(parent)
{
    form = new QGridLayout(this);
    setLayout(form);
    this->parent = parent;

    createSizeSettings(0);
    createButtons(2);

    resize(400, 200);
    setWindowModality(Qt::WindowModal);
    setWindowTitle("Konfiguracja rozmiaru siatki");
}


void GridSettingsWindow::createSizeSettings(int row)
{
    sizeForm = new QGroupBox(this);
    sizeForm->setTitle(tr("Rozmiar siatki"));
    sizeFormLayout = new QFormLayout(sizeForm);
    sizeForm->setLayout(sizeFormLayout);

    widthBox = new QSpinBox(this);
    widthBox->setMinimum(10);
    widthBox->setMaximum(200);
    sizeFormLayout->addRow(tr("Szerokość siatki"), widthBox);

    heightBox = new QSpinBox(this);
    heightBox->setMinimum(10);
    heightBox->setMaximum(200);
    sizeFormLayout->addRow(tr("Wysokość siatki"), heightBox);

    form->addWidget(sizeForm, row, 0, 1, 2);
}


void GridSettingsWindow::createButtons(int row)
{
    applyButton = new QPushButton(tr("&Zastosuj"), this);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applySettings()));
    form->addWidget(applyButton, row, 0);

    cancelButton = new QPushButton(tr("&Anuluj"), this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    form->addWidget(cancelButton, row, 1);
}


void GridSettingsWindow::applySettings()
{
    parent->viewer->resizeGrid(widthBox->value(), heightBox->value());
    close();
}


void GridSettingsWindow::readSettings()
{
    QSize s = parent->viewer->getGridSize();
    widthBox->setValue(s.width());
    heightBox->setValue(s.height());
}


void GridSettingsWindow::showEvent(QShowEvent *e)
{
    readSettings();
}

