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


#include "ColorSettingsDialog.hpp"
#include "CellInfo.hpp"

#include "MainWindow.hpp"


ColorPreview::ColorPreview(QWidget *parent) :
    QWidget(parent)
{
    painter = new QPainter(this);
}


void ColorPreview::setColor(int red, int green, int blue)
{
    color = QColor(red, green, blue);
    update();
}

void ColorPreview::paintEvent(QPaintEvent *e)
{
    painter->begin(this);
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawRect(0, 0, width(), height());
    painter->end();
}


ColorSettingsDialog::ColorSettingsDialog(MainWindow *parent) :
    QDialog(parent)
{
    mainWindow = parent;

    layout = new QFormLayout(this);
    setLayout(layout);
    layout->setAlignment(Qt::AlignTop);

    status = new QSpinBox(this);
    status->setMinimum(0);
    status->setMaximum(STATUS_NUMBER);
    connect(status, SIGNAL(valueChanged(int)), this, SLOT(updateRGB()));
    layout->addRow(tr("Status"), status);

    colorPreview = new ColorPreview(this);
    layout->addRow(tr("Podgląd"), colorPreview);

    red = new QSpinBox(this);
    red->setMinimum(0);
    red->setMaximum(255);
    connect(red, SIGNAL(valueChanged(int)), this, SLOT(updateColorPreview()));
    layout->addRow(tr("Czerwony"), red);

    green = new QSpinBox(this);
    green->setMinimum(0);
    green->setMaximum(255);
    connect(green, SIGNAL(valueChanged(int)), this, SLOT(updateColorPreview()));
    layout->addRow(tr("Zielony"), green);

    blue = new QSpinBox(this);
    blue->setMinimum(0);
    blue->setMaximum(255);
    connect(blue, SIGNAL(valueChanged(int)), this, SLOT(updateColorPreview()));
    layout->addRow(tr("Niebieski"), blue);

    resize(600, 400);
    setModal(true);

    updateRGB();
    updateColorPreview();
}


void ColorSettingsDialog::updateColorPreview()
{
    colorPreview->setColor(red->value(), green->value(), blue->value());
    mainWindow->setStatusFillColor(status->value(), QColor(red->value(), green->value(), blue->value()));
}


void ColorSettingsDialog::updateRGB()
{
    QColor c = mainWindow->getStatusFillColor(status->value());
    red->setValue(c.red());
    green->setValue(c.green());
    blue->setValue(c.blue());
}
