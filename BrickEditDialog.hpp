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


#ifndef BRICKEDITDIALOG_HPP
#define BRICKEDITDIALOG_HPP

#include <QDialog>
#include <QString>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QSpinBox>

#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"

#include <iostream>

typedef ScriptBrickIf::ComparisionOperators ComparisionOperator;



struct BrickBasicData
{
    QString left, right;
    ComparisionOperator op;
    StatusT status;
};


class BrickEditDialog : public QDialog
{
    Q_OBJECT

    BrickBasicData data;

    QGridLayout *layout;

    QLabel *labelLeft, *labelOperator, *labelRight, *labelColor;
    QComboBox *comboOperator;
    QSpinBox *selectColor;
    QLineEdit *editLeft, *editRight;
    QPushButton *buttonCancel, *buttonApply;

    void showEvent(QShowEvent *e);

public:
    explicit BrickEditDialog(QWidget *parent);

    void setData(BrickBasicData d);
    BrickBasicData getData();

private slots:
    void cancel();
    void apply();

};

#endif // BRICKEDITDIALOG_HPP
