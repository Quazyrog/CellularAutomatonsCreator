#ifndef POPULATIONVIEWWIDGET_HPP
#define POPULATIONVIEWWIDGET_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QSize>

#include <iostream>

class MainWindow;


#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"


class PopulationViewWidget : public QWidget
{
    Q_OBJECT
private:
    MainWindow *mainWindow;

    QPainter *painter;
    QPen normalPen,  highlightedPen;
    QColor colors[5];
    int cursorCellX, cursorCellY;
    void countDrawParams();

    QRect gridArea;
    int cellSize, hmargin, vmargin;
    QSize gridSize;
    CellularAutomaton *world;
    const int minMargin = 5;

    StatusT statusBrush;

    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *e);

public:
    explicit PopulationViewWidget(MainWindow *parent, CellularAutomaton *automaton);
    virtual ~PopulationViewWidget();

    void resizeGrid(size_t width, size_t height);
    QSize getGridSize();

public slots:
    void nextGeneration();
    void resetGrid();
    void makeRandomGrid();
    void setBrush(StatusT brush);

};

#endif // POPULATIONVIEWWIDGET_HPP
