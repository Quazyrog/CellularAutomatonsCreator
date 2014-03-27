#ifndef SCRIPTVIEWWIDGET_HPP
#define SCRIPTVIEWWIDGET_HPP

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>
#include <QFont>
#include <QInputDialog>

#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"
#include "Exceptions.hpp"
#include "BrickEditDialog.hpp"


typedef ScriptBrickIf::ComparisionOperators ComparisionOperator;


class MainWindow;


class ScriptViewWidget : public QWidget
{
    Q_OBJECT

    struct Brick {
        QString left, right;
        ComparisionOperator op;
        StatusT status;

        int column;

        Brick *under, *after, *parent;
        Brick *prev, *next;

        ~Brick();
    };

    Brick *createBrick(QString left, ComparisionOperator op, QString right, StatusT color);
    void addBrickToList(Brick *after, Brick *toInsert);
    QString getBrickText(Brick *b);
    QColor getBrickColor(Brick *b);

    int rows, columns, selectionRow;

    int brickWidth, brickHeight;
    int textSize;
    int horizontalMargin, verticalMargin;
    int horizontalSpacing, verticalSpacing;

    StatusT defaultStatus;

    QPainter *painter;
    QFont textFont;

    Brick *firstBrick, *selected;

    MainWindow *mainWindow;

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void mkSize(bool recountRows = false);

    ScriptBrick *compileBrick(Brick *brick);

public:
    explicit ScriptViewWidget(MainWindow *mainWindow, QWidget *parent = 0);
    ~ScriptViewWidget();

    int getBrickWidth();
    void setBrickWidth(int value);

    int getBrickHeight();
    void setBrickHeight(int value);

    int getTextSize();
    void setTextSize(int value);

    int getVerticalMargin();
    void setVerticalMargin(int value);

    int getHorizontalMargin();
    void setHorizontalMargin(int value);

    int getHorizontalSpacing();
    void setHorizontalSpacing(int value);

    int getVerticalSpacing();
    void setVerticalSpacing(int value);

    ScriptBrick *compile();

public slots:
    void eraseSelected();
    void editSelected();
    void addUnderSelected();
    void addAfterSelected();
    void editDefault();

};

#endif // SCRIPTVIEWWIDGET_HPP
