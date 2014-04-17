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


#ifndef AUTOMATONSCRIPTEDITOR_HPP
#define AUTOMATONSCRIPTEDITOR_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

#include "ScriptViewWidget.hpp"

class MainWindow;
class AutomatonScriptEditor;


class ScriptButtonsWidget : public QWidget
{
    Q_OBJECT

    QVBoxLayout *layout;
    QPushButton *addNext, *addSub, *remove, *edit, *setDefault, *installScript;
public:
    ScriptButtonsWidget(AutomatonScriptEditor *parent);
};


class AutomatonScriptEditor : public QDialog
{
    Q_OBJECT

    friend class ScriptButtonsWidget;

    QHBoxLayout *mainLayout;
    ScriptButtonsWidget *buttonsWidget;

    ScriptViewWidget *scriptView;
    QScrollArea *scriptViewScrollArea;
    QList<QString> *script;

    MainWindow *mainWindow;

public:
    AutomatonScriptEditor(MainWindow *parent);

public slots:
    void installScript();
};

#endif // AUTOMATONSCRIPTEDITOR_HPP
