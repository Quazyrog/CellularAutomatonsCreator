#include "ScriptViewWidget.hpp"

#include "MainWindow.hpp"


ScriptViewWidget::Brick::~Brick()
{
    if (under != nullptr)
        delete under;
}


ScriptViewWidget::ScriptViewWidget(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent)
{
    brickHeight = 30;
    brickWidth = 300;
    textSize = 10;
    horizontalMargin = 5;
    verticalMargin = 5;
    horizontalSpacing = 50;
    verticalSpacing = 3 + brickHeight;
    defaultStatus = 0;

    rows = 1;
    columns = 0;

    setMinimumSize(brickWidth + horizontalMargin * 2 + horizontalSpacing * 5, verticalSpacing * 10);

    painter = new QPainter(this);
    textFont.setPointSize(textSize);

    this->mainWindow = mainWindow;

    firstBrick = createBrick("0", ScriptBrickIf::EQUAL, "0", 0);
    firstBrick->column = 0;
    selected = firstBrick;
}


ScriptViewWidget::~ScriptViewWidget()
{
    QList<Brick *> bricksToDelete;
    for (Brick *b = firstBrick; b != nullptr; b = b->next) {
        if (b->column == 0) {
            bricksToDelete.append(b);
        }
    }

    for (auto it = bricksToDelete.begin(); it != bricksToDelete.end(); it++) {
        delete *it;
    }
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


void ScriptViewWidget::eraseSelected()
{
    if (rows == 1) {
        QMessageBox err(this);
        err.setIcon(QMessageBox::Critical);
        err.setWindowTitle(tr("Błąd"));
        err.setText(tr("Nie można usunąc ostatniego bloku"));
        err.exec();
        return;
    }


    QMessageBox confirm(this);
    confirm.setIcon(QMessageBox::Question);
    confirm.setWindowTitle(tr("Pytanie"));
    confirm.setText(tr("Czy na pewno chcesz usunąc zaznaczony blok?"));
    confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(confirm.exec() == QMessageBox::Rejected) {
        return;
    }



    Brick *newSelection;
    if (firstBrick == selected) {
        firstBrick = selected->after;
        delete selected;
        newSelection = firstBrick;
    } else if (selected->column == 0) {
        newSelection = selected->parent;

        /* Remove from tree */
        selected->parent->after = selected->after;
        if (selected->after != nullptr) {
            selected->after->parent = selected->parent;
        }

        /* Remove from list */
        Brick *deepest;
        for (deepest = selected; deepest->under != nullptr; deepest = deepest->under);
        if (deepest->next != nullptr) {
            deepest->next->prev = selected->prev;
        }
        if (selected->prev != nullptr) {
            selected->prev->next = deepest->next;
        }

        delete selected;
    } else {
        newSelection = selected->parent;

        /* Remove from tree */
        selected->parent->under = selected->after;

        /* Remove from list */
        Brick *deepest;
        for (deepest = selected; deepest->under != nullptr; deepest = deepest->under);
        if (deepest->next != nullptr) {
            deepest->next->prev = selected->prev;
        }
        if (selected->prev != nullptr) {
            selected->prev->next = deepest->next;
        }

        delete selected;
    }
    selected = newSelection;

    mkSize(true);
}


void ScriptViewWidget::addUnderSelected()
{
    BrickBasicData data = {"", "", ComparisionOperator::EQUAL, 0};
    BrickEditDialog dialog(this);
    dialog.setData(data);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    data = dialog.getData();
    Brick *inserted = createBrick(data.left, data.op, data.right, data.status);
    inserted->column = selected->column + 1;

    addBrickToList(selected, inserted);

    inserted->after = selected->under;
    selected->under = inserted;
    inserted->parent = selected;

    rows++;
    columns = std::max(columns, inserted->column);
    selected = inserted;
    mkSize();
}


void ScriptViewWidget::addAfterSelected()
{
    BrickBasicData data;
    BrickEditDialog dialog(this);
    dialog.setData(data);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    data = dialog.getData();
    Brick *inserted = createBrick(data.left, data.op, data.right, data.status), *lastSon;
    inserted->column = selected->column;

    if (selected->under != nullptr) {
        lastSon = selected->under;
        for (; lastSon->after != nullptr; lastSon = lastSon->after);
        for (; lastSon->under != nullptr; lastSon = lastSon->under);
        addBrickToList(lastSon, inserted);
    } else {
        addBrickToList(selected, inserted);
    }

    inserted->after = selected->after;
    selected->after = inserted;
    inserted->parent = selected;

    rows++;
    selected = inserted;
    mkSize();
}


void ScriptViewWidget::paintEvent(QPaintEvent *e)
{
    painter->begin(this);

    QColor bgColor = mainWindow->getStatusFillColor(defaultStatus);
    bgColor.setRed((bgColor.red() + 255) / 2);
    bgColor.setGreen((bgColor.green() + 255) / 2);
    bgColor.setBlue((bgColor.blue() + 255) / 2);
    painter->setBrush(bgColor);
    painter->drawRect(0, 0, width(), height());

    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBackground(Qt::white);
    painter->setPen(Qt::black);
    painter->setFont(textFont);

    int row = 0;
    for (Brick *painted = firstBrick; painted != nullptr; painted = painted->next) {
        int x, y;
        x = painted->column * horizontalSpacing + horizontalMargin;
        y = row * verticalSpacing + verticalMargin;
        painter->setBrush(getBrickColor(painted));
        if (painted == selected) {
            QPen pen(QColor(0xEA, 0xA6, 0x08));
            pen.setWidth(3);

            painter->save();
            painter->setPen(pen);
            painter->drawRect(x, y, brickWidth, brickHeight);
            painter->restore();
            painter->drawText(x + horizontalMargin, y, brickWidth - horizontalMargin - horizontalSpacing, brickHeight,
                              Qt::AlignVCenter | Qt::AlignLeft, getBrickText(painted));
        } else {
            painter->drawRect(x, y, brickWidth, brickHeight);
            painter->drawText(x + horizontalMargin, y, brickWidth - horizontalMargin - horizontalSpacing, brickHeight,
                              Qt::AlignVCenter | Qt::AlignLeft, getBrickText(painted));
        }

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
    brick->parent = nullptr;
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


QString ScriptViewWidget::getBrickText(Brick *b)
{
    QString opSgn;
    switch (b->op) {
    case ScriptBrickIf::EQUAL :
        opSgn = "=";
        break;
    case ScriptBrickIf::NOT_EQUAL :
        opSgn = "≠";
        break;
    case ScriptBrickIf::LESS :
        opSgn = "<";
        break;
    case ScriptBrickIf::LESS_OR_EQUAL :
        opSgn = "≤";
        break;
    case ScriptBrickIf::GREATER :
        opSgn = ">";
        break;
    case ScriptBrickIf::GREATER_OR_EQUAL :
        opSgn = "≥";
        break;
    }

    return b->left + opSgn + b->right;
}


void ScriptViewWidget::mkSize(bool recountRows)
{
    if (recountRows) {
        rows = 0;
        for (Brick *b = firstBrick; b != nullptr; b = b->next) {
            rows++;
        }
    }

    int w, h;
    w = (columns - 1) * horizontalSpacing + brickWidth + 2 * horizontalMargin;
    h = rows * verticalSpacing + 2 * verticalMargin;
    resize(w, h);
    repaint();
}


void ScriptViewWidget::mousePressEvent(QMouseEvent *e)
{
    int y = selectionRow * verticalSpacing + verticalMargin;

    if (y <= e->y() && e->y() <= y + verticalMargin) {
        return;
    }

    int row = (e->y() - verticalMargin) / verticalSpacing;
    Brick *newSelected = firstBrick;
    for (int i = 0; i < row; i++) {
        if (newSelected->next == nullptr) {
            break;
        }
        newSelected = newSelected->next;
    }

    selected = newSelected;
    repaint();
}


void ScriptViewWidget::editSelected()
{
    BrickBasicData data;
    data.left = selected->left;
    data.right = selected->right;
    data.op = selected->op;
    data.status = selected->status;

    BrickEditDialog dialog(this);
    dialog.setData(data);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    data = dialog.getData();

    selected->left = data.left;
    selected->right = data.right;
    selected->op = data.op;
    selected->status = data.status;

    repaint();
}


void ScriptViewWidget::editDefault()
{
    bool ok;
    int val = QInputDialog::getInt(this, tr("Edycja domyślnego stanu"),
                                   tr("Podaj domyślny stan komórki:"), defaultStatus, 0, STATUS_NUMBER - 1, 1, &ok);
    if (ok && val != defaultStatus) {
        defaultStatus = val;
        repaint();
    }
}


ScriptBrick *ScriptViewWidget::compile()
{
    return compileBrick(firstBrick);
}


ScriptBrick *ScriptViewWidget::compileBrick(Brick *brick)
{
    if (brick == nullptr) {
        return new ScriptBrickReturn(defaultStatus);
    }

    ScriptBrickIf *scriptBlock = new ScriptBrickIf(brick->left, brick->op, brick->right);
    if (brick->under != nullptr) {
        scriptBlock->then = compileBrick(brick->under);
    } else {
        scriptBlock->then = new ScriptBrickReturn(brick->status);
    }

    scriptBlock->next = compileBrick(brick->after);
    return scriptBlock;
}
