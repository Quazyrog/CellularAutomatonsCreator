/* Copyright 2015 Wojciech Matusiak
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

#include "StateSwitchWindow.hpp"
#include <iostream>

#include "CellularAutomaton.hpp"
#include "GridViewer.hpp"


StateInfoWidget::StateInfoWidget(QWidget *parent):
    QWidget(parent)
{
    _automaton = nullptr;

    _numberDisplay = new QLabel(this);
    _numberLabel = new QLabel( "State number (quint16):", this);
    _nameDisplay = new QLabel(this);
    _nameLabel = new QLabel("State name:", this);
    _colorDisplay = new QLabel(this);
    _colorLabel = new QLabel("State display color RGB:" ,this);

    _layout = new QFormLayout(this);
    _layout->setLabelAlignment(Qt::AlignLeft);
    setLayout(_layout);
    _layout->addRow(_numberLabel, _numberDisplay);
    _layout->addRow(_nameLabel, _nameDisplay);
    _layout->addRow(_colorLabel, _colorDisplay);

    updateDisplays(0);
}


void StateInfoWidget::updateDisplays(quint16 state)
{
    if (_automaton == nullptr)
        return;

    _numberDisplay->setText(QString::number(state));
    _nameDisplay->setText(_automaton->stateName(state));
    const QColor color = _automaton->stateColor(state);
    _colorDisplay->setText(QString().sprintf("(%hu; %hu; %hu)", color.red(), color.green(), color.blue()));
}


void StateInfoWidget::setAutomaton(Scripting::CellularAutomaton *automaton)
{
    _automaton = automaton;
    updateDisplays(0);
}


StatesTableModel::StatesTableModel(QObject *parent):
    QAbstractListModel(parent)
{
    _automaton = nullptr;
}


int StatesTableModel::rowCount(const QModelIndex &) const
{
    if (_automaton == nullptr)
        return 0;
    return _automaton->statesNumber();
}


QVariant StatesTableModel::data(const QModelIndex &index, int role) const
{
    if (_automaton == nullptr)
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return QString().sprintf("%i   ", index.row()) + _automaton->stateName(index.row());
    case Qt::BackgroundColorRole:
        return _automaton->stateColor(index.row());
    }

    return QVariant();
}



void StatesTableModel::statesModified(quint16 state)
{
    QModelIndex index = createIndex(state, 0);
    emit(dataChanged(index, index));
}


void StatesTableModel::setAutomaton(Scripting::CellularAutomaton *automaton)
{
    _automaton = automaton;
    if (_automaton == nullptr)
        return;
    emit(dataChanged(createIndex(0, 0), createIndex(_automaton->statesNumber(), 0)));
}


StateSwitchWidget::StateSwitchWidget(GridViewer *viewer, QWidget *parent) :
    QWidget(parent)
{
    _viewer = viewer;

    _statesModel = new StatesTableModel(this);

    _layout = new QVBoxLayout(this);

    _infoWidget = new StateInfoWidget(this);
    _layout->addWidget(_infoWidget);

    _palette = new QListView(this);
    _palette->setModel(_statesModel);
    connect(_palette->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
            this, SLOT(selectedBrushChange(QItemSelection,QItemSelection)));
    _layout->addWidget(_palette);
}


void StateSwitchWidget::selectedBrushChange(const QItemSelection &, const QItemSelection &)
{
    quint16 newBrush = _palette->selectionModel()->currentIndex().row();
    _viewer->setStateBrush(newBrush);
    _infoWidget->updateDisplays(newBrush);
}


void StateSwitchWidget::setAutomaton(Scripting::CellularAutomaton *automaton)
{
    _infoWidget->setAutomaton(automaton);
    _statesModel->setAutomaton(automaton);
}


StateSwitchDock::StateSwitchDock(GridViewer *viewer, QWidget *parent):
    QDockWidget(parent)
{
    _widget = new StateSwitchWidget(viewer, this);
    setWidget(_widget);
    setMinimumSize(150, 300);
    setWindowTitle("States palette");
    setObjectName("StateSwitchDock");
}


void StateSwitchDock::setAutomaton(Scripting::CellularAutomaton *automaton)
{
    qDebug() << "Changind switch's automaton to" << static_cast<void*>(automaton);
    _widget->setAutomaton(automaton);
}
