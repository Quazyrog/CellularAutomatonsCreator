#ifndef POPULATIONVIEWWIDGET_HPP
#define POPULATIONVIEWWIDGET_HPP

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <iostream>

class Exception;
struct CellInfo;



class PopulationViewWidget : public QWidget
{
    Q_OBJECT
private:
    QPainter *painter;
    QPen normalPen,  highlightedPen;
    QColor colors[5];
    int cursorCellX, cursorCellY;
    void countDrawParams();

    QSize gridSize;
    QRect gridArea;
    int cellSize, hmargin, vmargin;
    const int minMargin = 5;
    char **oldGrid, **grid;
    void deleteGrid();
    void resizeGrid(int width, int height);

    char statusBrush, currentMaxStatus;
    CellInfo examineCell(int x, int y);
    char checkNewStatus(int x, int y);
    void repaintCell(int x, int y, QPen pen);

    virtual void paintEvent(QPaintEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *e);

    QMenu *contextMenu;
    QAction *setStatus0, *setStatus1, *setStatus2, *setStatus3, *setStatus4;
    QSignalMapper *contextMenuSignalMapper;
    void createContextMenu();
    virtual void contextMenuEvent(QContextMenuEvent *e);

    int min(int a, int b) {
        return (a < b ? a : b);
    }

    int max(int a, int b) {
        return (a > b ? a : b);
    }


public:
    explicit PopulationViewWidget(QWidget *parent = 0, int width = 40, int height = 20);
    virtual ~PopulationViewWidget();

    void setStatusColor(unsigned int s, QColor c);
    QColor getStatusColor(unsigned int s);

    void setGridSize(int width, int height);
    QSize getGridSize();


public slots:
    void nextGeneration();
    void resetGrid();
    void makeRandomGrid();
    void setBrush(int brush);

};

#endif // POPULATIONVIEWWIDGET_HPP
