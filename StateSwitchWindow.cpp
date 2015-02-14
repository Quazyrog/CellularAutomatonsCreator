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


StateInfoWidget::StateInfoWidget(Scripting::CellularAutomaton *automaton, QWidget *parent):
    QWidget(parent)
{
    _automaton = automaton;

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
    _numberDisplay->setText(QString::number(state));
    _nameDisplay->setText(_automaton->getStateName(state));
    const QColor color = _automaton->getStateColor(state);
    _colorDisplay->setText(QString().sprintf("(%hu; %hu; %hu)", color.red(), color.green(), color.blue()));
}


StatesTableModel::StatesTableModel(Scripting::CellularAutomaton *automaton, QObject *parent):
    QAbstractListModel(parent)
{
    _automaton = automaton;
}


int StatesTableModel::rowCount(const QModelIndex &) const
{
    return _automaton->getStatesNumber();
}


QVariant StatesTableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString().sprintf("%i   ", index.row()) + _automaton->getStateName(index.row());
    case Qt::BackgroundColorRole:
        return _automaton->getStateColor(index.row());
    }
    return QVariant();
}



void StatesTableModel::statesModified(quint16 state)
{
    QModelIndex index = createIndex(state, 1);
    emit(dataChanged(index, index));
}


StateSwitchWidget::StateSwitchWidget(Scripting::CellularAutomaton *automaton, GridViewer *viewer, QWidget *parent) :
    QWidget(parent)
{
    _statesModel = new StatesTableModel(automaton, this);
    _viewer = viewer;

    _layout = new QVBoxLayout(this);

    _infoWidget = new StateInfoWidget(automaton, this);
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


StateSwitchDock::StateSwitchDock(Scripting::CellularAutomaton *automaton, GridViewer *viewer, QWidget *parent):
    QDockWidget(parent)
{
    _widget = new StateSwitchWidget(automaton, viewer, this);
    setWidget(_widget);
    setMinimumSize(150, 300);
    setWindowTitle("States palette");
    setObjectName("StateSwitchDock");
}
