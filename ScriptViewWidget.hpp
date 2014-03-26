#ifndef SCRIPTVIEWWIDGET_HPP
#define SCRIPTVIEWWIDGET_HPP

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>
#include <QFont>

#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"
#include "Exceptions.hpp"


typedef CellularAutomaton::ScriptBrickIf::ComparisionOperators ComparisionOperator;


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
    };

    Brick *createBrick(QString left, ComparisionOperator op, QString right, StatusT color);
    void addBrickToList(Brick *after, Brick *toInsert);
    QString getBrickText(Brick *b);
    QColor getBrickColor(Brick *b);
    QColor getTextColor(Brick *b);

    int rows, columns;

    int brickWidth, brickHeight;
    int textSize;
    int horizontalMargin, verticalMargin;
    int horizontalSpacing, verticalSpacing;

    QPainter *painter;
    QFont textFont;

    Brick *firstBrick, *selection;

    MainWindow *mainWindow;

    void paintEvent(QPaintEvent *e);

public:
    explicit ScriptViewWidget(MainWindow *mainWindow, QWidget *parent = 0);

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

public slots:
    void selectBrick(int row, int column);
    void eraseSelected();
    void addUnderSelected(QString left, ComparisionOperator op, QString right, StatusT status);
    void addAfterSelected(QString left, ComparisionOperator op, QString right, StatusT status);

};

#endif // SCRIPTVIEWWIDGET_HPP
