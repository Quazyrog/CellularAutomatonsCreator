#include "PopulationViewWidget.hpp"
#include "Exception.hpp"
#include "CellInfo.hpp"



PopulationViewWidget::PopulationViewWidget(QWidget *parent, int width, int height) :
    QWidget(parent)
{
    painter = new QPainter(this);

    colors[0] = QColor(Qt::lightGray);
    colors[1] = QColor(Qt::black);
    colors[2] = QColor(Qt::red);
    colors[3] = QColor(Qt::green);
    colors[4] = QColor(Qt::blue);

    normalPen.setWidth(1);
    normalPen.setColor(Qt::black);
    highlightedPen.setWidth(2);
    highlightedPen.setColor(Qt::yellow);

    setMouseTracking(true);
    createContextMenu();
    cursorCellX = cursorCellY = -1;
    statusBrush = 1;
    currentMaxStatus = 1;

    gridSize.setWidth(max(width, 0));
    gridSize.setHeight(max(height, 0));
    grid = new char* [gridSize.width()];
    oldGrid = new char* [gridSize.width()];
    for (int x = 0; x < gridSize.width(); x++) {
        grid[x] = new char[height];
        oldGrid[x] = new char[height];
        memset(grid[x], 0, height);
        memset(oldGrid[x], 0, height);
    }
}


PopulationViewWidget::~PopulationViewWidget() {
    deleteGrid();
}


void PopulationViewWidget::paintEvent(QPaintEvent *e)
{
    painter->begin(this);

    QRect updateArea;
    updateArea.setX((e->rect().x() - hmargin) / cellSize);
    updateArea.setX((updateArea.x() < 0 ? 0 : updateArea.x()));
    updateArea.setY((e->rect().y() - vmargin) / cellSize);
    updateArea.setY((updateArea.y() < 0 ? 0 : updateArea.y()));
    updateArea.setWidth(e->rect().width() / cellSize);
    updateArea.setHeight(e->rect().height() / cellSize);

    int xlim = updateArea.x() + updateArea.width(), ylim = updateArea.y() + updateArea.height();
    xlim = (xlim > gridSize.width() ? gridSize.width() : xlim);
    ylim = (ylim > gridSize.height() ? gridSize.height() : ylim);
    for (int x = updateArea.x(); x < xlim; x++) {
        for (int y = updateArea.y(); y < ylim; y++) {
            painter->setBrush(getStatusColor(grid[x][y]));
            painter->drawRect(hmargin + x * cellSize, vmargin + y * cellSize, cellSize, cellSize);
        }
    }

    if (0 <= cursorCellX && cursorCellX < gridSize.width() && 0 <= cursorCellY && cursorCellY < gridSize.height()) {
        QColor c1(getStatusColor(grid[cursorCellX][cursorCellY])), c2(getStatusColor(statusBrush)), cmix;
        cmix.setRed((c1.red() + c2.red()) / 2);
        cmix.setGreen((c1.green() + c2.green()) / 2);
        cmix.setBlue((c1.blue() + c2.blue()) / 2);

        painter->setPen(highlightedPen);
        painter->setBrush(QBrush(cmix));
        painter->drawRect(hmargin + cursorCellX * cellSize, vmargin + cursorCellY * cellSize, cellSize, cellSize);
    }

    painter->end();
}


CellInfo PopulationViewWidget::examineCell(int x, int y)
{
    CellInfo result;
    for (int nx = x - 1; nx <= x + 1; nx++) {
        for (int ny = y - 1; ny <= y + 1; ny++) {
            if (0 <= nx && nx <= gridSize.width() - 1 && 0 <= ny && ny <= gridSize.height() - 1 && !(nx == x && ny == y))
                result(oldGrid[nx][ny])++;
        }
    }
    return result;
}


char PopulationViewWidget::checkNewStatus(int x, int y)
{
    CellInfo stat = examineCell(x, y);
    if (oldGrid[x][y] == 1) {
        if (stat(1) == 2 || stat(1) == 3)
            return 1;
        else
            return 0;
    } else {
        if (stat(1) == 3)
            return 1;
        else
            return 0;
    }
}


void PopulationViewWidget::nextGeneration()
{
    char **g;
    g = oldGrid;
    oldGrid = grid;
    grid = g;
    for (int x = 0; x < gridSize.width(); x++) {
        for (int y = 0; y < gridSize.height(); y++) {
            grid[x][y] = checkNewStatus(x, y);
        }
    }

    update(0, 0, width(), height());
}


void PopulationViewWidget::mouseMoveEvent(QMouseEvent *e)
{
    int oldx = cursorCellX ;
    cursorCellX = (e->x() - hmargin) / cellSize;
    int oldy = cursorCellY;
    cursorCellY = (e->y() - vmargin) / cellSize;

    if (oldx != cursorCellX || oldy != cursorCellY) {
        update((oldx - 1) * cellSize + hmargin, (oldy - 1) * cellSize + vmargin, cellSize * 3, cellSize * 3);
    }
}


void PopulationViewWidget::resizeEvent(QResizeEvent * e)
{
    countDrawParams();
}

void PopulationViewWidget::mousePressEvent(QMouseEvent *e)
{
    if (cursorCellX < 0 || cursorCellX >= gridSize.width() || cursorCellY < 0 || cursorCellY >= gridSize.height())
        return;
    if (e->button() != Qt::LeftButton)
        return;
    grid[cursorCellX][cursorCellY] = statusBrush;
    update((cursorCellX - 1) * cellSize + hmargin, (cursorCellY - 1) * cellSize + vmargin, cellSize *3, cellSize * 3);
}


void PopulationViewWidget::repaintCell(int x, int y, QPen pen)
{
    if (0 <= x && x < gridSize.width() && 0 <= y && y < gridSize.height()) {
        bool painterWasActivated;
        if (!painter->isActive()) {
            painterWasActivated = true;
            painter->begin(this);
        }
        painter->setPen(pen);
        painter->setBrush(getStatusColor(grid[x][y]));
        painter->drawRect(hmargin + x * cellSize, vmargin + y * cellSize, cellSize, cellSize);
        if (painterWasActivated)
            painter->end();
    }
}


void PopulationViewWidget::resetGrid()
{
    for (int x = 0; x < gridSize.width(); x++)
        memset(grid[x], 0, gridSize.height());
    update(0, 0, width(), height());
}


void PopulationViewWidget::makeRandomGrid()
{
    for (int x = 0; x < gridSize.width(); x++) {
        for (int y = 0; y < gridSize.height(); y++) {
            grid[x][y] = rand() % (currentMaxStatus + 1);
        }
    }
    update(0, 0, width(), height());
}


void PopulationViewWidget::createContextMenu()
{
    setStatus0 = new QAction(tr("Stan &0"), this);
    setStatus1 = new QAction(tr("Stan &1"), this);
    setStatus2 = new QAction(tr("Stan &2"), this);
    setStatus3 = new QAction(tr("Stan &3"), this);
    setStatus4 = new QAction(tr("Stan &4"), this);

    contextMenu = new QMenu(tr("Wybierz stan pędzla"), this);
    contextMenu->addAction(setStatus0);
    contextMenu->addAction(setStatus1);
    contextMenu->addAction(setStatus2);
    contextMenu->addAction(setStatus3);
    contextMenu->addAction(setStatus4);

    contextMenuSignalMapper = new QSignalMapper(this);
    contextMenuSignalMapper->setMapping(setStatus0, 0);
    contextMenuSignalMapper->setMapping(setStatus1, 1);
    contextMenuSignalMapper->setMapping(setStatus2, 2);
    contextMenuSignalMapper->setMapping(setStatus3, 3);
    contextMenuSignalMapper->setMapping(setStatus4, 4);

    connect(setStatus0, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));
    connect(setStatus1, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));
    connect(setStatus2, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));
    connect(setStatus3, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));
    connect(setStatus4, SIGNAL(triggered()), contextMenuSignalMapper, SLOT(map()));

    connect(contextMenuSignalMapper, SIGNAL(mapped(int)), this, SLOT(setBrush(int)));
}


void PopulationViewWidget::setBrush(int brush)
{
    if (brush < 0 || brush > 4)
        return;
    statusBrush = brush;
}


void PopulationViewWidget::contextMenuEvent(QContextMenuEvent *e)
{
    contextMenu->exec(e->globalPos());
}


void PopulationViewWidget::setStatusColor(unsigned int s, QColor c) {
    if (s <= 4)
        colors[s] = c;
}


QColor PopulationViewWidget::getStatusColor(unsigned int s) {
    if (s <= 4)
        return colors[s];
    return QColor();
}


void PopulationViewWidget::deleteGrid()
{
    for (int x = 0; x < gridSize.width(); x++) {
        delete [] grid[x];
        delete [] oldGrid[x];
    }
    delete [] grid;
    delete [] oldGrid;
}


void PopulationViewWidget::resizeGrid(int width, int height)
{
    deleteGrid();

    gridSize.setWidth(max(width, 0));
    gridSize.setHeight(max(height, 0));
    grid = new char* [gridSize.width()];
    oldGrid = new char* [gridSize.width()];
    for (int x = 0; x < gridSize.width(); x++) {
        grid[x] = new char[height];
        oldGrid[x] = new char[height];
        memset(grid[x], 0, height);
        memset(oldGrid[x], 0, height);
    }

    gridSize.setWidth(width);
    gridSize.setHeight(height);
}


QSize PopulationViewWidget::getGridSize()
{
    return gridSize;
}

void PopulationViewWidget::setGridSize(int width, int height)
{
    if (width < 0 || height < 0) return;
    if (width == gridSize.width() && height == gridSize.height()) return;
    resizeGrid(width, height);
    gridSize = QSize(width, height);
    countDrawParams();
    repaint();
}


void PopulationViewWidget::countDrawParams()
{
    cellSize = min((width() - 2 * minMargin) / gridSize.width(), (height() - 2 * minMargin) / gridSize.height());
    hmargin = (width() - cellSize * gridSize.width()) / 2;
    vmargin = (height() - cellSize * gridSize.height()) / 2;
}
