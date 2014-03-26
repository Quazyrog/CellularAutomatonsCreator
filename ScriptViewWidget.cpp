#include "ScriptViewWidget.hpp"

#include "MainWindow.hpp"


ScriptViewWidget::ScriptViewWidget(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent)
{
    brickHeight = 30;
    brickWidth = 300;
    textSize = 14;
    horizontalMargin = 5;
    verticalMargin = 5;
    horizontalSpacing = 50;
    verticalSpacing = 3 + brickHeight;

    rows = 0;
    columns = 0;

    setMinimumSize(brickWidth + horizontalMargin * 2 + horizontalSpacing * 5, verticalSpacing * 10);

    painter = new QPainter(this);
    textFont.setPointSize(textSize);

    this->mainWindow = mainWindow;

    firstBrick = createBrick("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    firstBrick->column = 0;

    selection = firstBrick;
    addUnderSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    addUnderSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    addUnderSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    addAfterSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    addAfterSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
    addAfterSelected("0", CellularAutomaton::ScriptBrickIf::EQUAL, "0", 0);
}


int ScriptViewWidget::getBrickWidth()
{
    return brickWidth;
}


void ScriptViewWidget::setBrickWidth(int value)
{
    if (value > 0)
        brickWidth = value;
}


int ScriptViewWidget::getBrickHeight()
{
    return brickHeight;
}


void ScriptViewWidget::setBrickHeight(int value)
{
    if (value > 0)
        brickHeight = value;
}


int ScriptViewWidget::getTextSize()
{
    return textSize;
}


void ScriptViewWidget::setTextSize(int value)
{
    if (value > 0)
        textSize = value;
}


int ScriptViewWidget::getVerticalMargin()
{
    return verticalMargin;
}


void ScriptViewWidget::setVerticalMargin(int value)
{
    if (value > 0)
        verticalMargin = value;
}


int ScriptViewWidget::getHorizontalMargin()
{
    return horizontalMargin;
}


void ScriptViewWidget::setHorizontalMargin(int value)
{
    if (value > 0)
        horizontalMargin = value;
}


int ScriptViewWidget::getVerticalSpacing()
{
    return verticalSpacing;
}


void ScriptViewWidget::setVerticalSpacing(int value)
{
    if (value > 0)
        verticalSpacing = value;
}


int ScriptViewWidget::getHorizontalSpacing()
{
    return horizontalSpacing;
}


void ScriptViewWidget::setHorizontalSpacing(int value)
{
    if (value > 0)
        horizontalSpacing = value;
}


void ScriptViewWidget::selectBrick(int row, int column)
{

}


void ScriptViewWidget::eraseSelected()
{

}


void ScriptViewWidget::addUnderSelected(QString left, ComparisionOperator op, QString right, StatusT status)
{
    Brick *inserted = createBrick(left, op, right, status);
    inserted->column = selection->column + 1;

    addBrickToList(selection, inserted);

    inserted->after = selection->under;
    selection->under = inserted;
    inserted->parent = selection;

    rows++;
    columns = std::max(columns, inserted->column);
}


void ScriptViewWidget::addAfterSelected(QString left, ComparisionOperator op, QString right, StatusT status)
{
    Brick *inserted = createBrick(left, op, right, status), *lastSon;
    inserted->column = selection->column;

    for (lastSon = selection; lastSon->under != nullptr; lastSon = lastSon->under);
    addBrickToList(lastSon, inserted);

    inserted->after = selection->after;
    selection->after = inserted;
    inserted->parent = selection;

    rows++;
}


void ScriptViewWidget::paintEvent(QPaintEvent *e)
{
    painter->begin(this);

    painter->setBackground(Qt::white);
    painter->setPen(Qt::black);
    painter->setFont(textFont);

    int row = 0;
    for (Brick *painted = firstBrick; painted != nullptr; painted = painted->next) {
        int x, y;
        x = painted->column * horizontalSpacing + horizontalMargin;
        y = row * verticalSpacing + verticalMargin;
        painter->setBrush(getBrickColor(painted));
        painter->drawRect(x, y, brickWidth, brickHeight);
        painter->setPen(getTextColor(painted));
        painter->drawText(x, y, x + brickWidth, y + brickHeight, Qt::AlignVCenter | Qt::AlignLeft, getBrickText(painted));
        row++;
    }

    painter->end();
}


ScriptViewWidget::Brick *ScriptViewWidget::createBrick(QString left, ComparisionOperator op, QString right, StatusT status)
{
    Brick *brick = new Brick;
    brick->left = left;
    brick->op = op;
    brick->right = right;
    brick->status = status;
    brick->after = nullptr;
    brick->under = nullptr;
    brick->prev = nullptr;
    brick->next = nullptr;
    brick->column = -100;
    return brick;
}


void ScriptViewWidget::addBrickToList(Brick *after, Brick *toInsert)
{
    Brick *next = after->next;

    after->next = toInsert;
    toInsert->prev = after;

    toInsert->next = next;
    if (next != nullptr) {
        next->prev = toInsert;
    }
}


QColor ScriptViewWidget::getBrickColor(Brick *b)
{
    return mainWindow->getStatusFillColor(b->status);
}

QColor ScriptViewWidget::getTextColor(Brick *b)
{
    return mainWindow->getStatusTextColor(b->status);
}


QString ScriptViewWidget::getBrickText(Brick *b)
{
    QString opSgn;
    switch (b->op) {
    case CellularAutomaton::ScriptBrickIf::EQUAL :
        opSgn = "=";
        break;
    case CellularAutomaton::ScriptBrickIf::NOT_EQUAL :
        opSgn = "≠";
        break;
    case CellularAutomaton::ScriptBrickIf::LESS :
        opSgn = "<";
        break;
    case CellularAutomaton::ScriptBrickIf::LESS_OR_EQUAL :
        opSgn = "≤";
        break;
    case CellularAutomaton::ScriptBrickIf::GREATER :
        opSgn = ">";
        break;
    case CellularAutomaton::ScriptBrickIf::GREATER_OR_EQUAL :
        opSgn = "≥";
        break;
    }

    return b->left + opSgn + b->right;
}
