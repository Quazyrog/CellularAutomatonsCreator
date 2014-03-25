#ifndef SCRIPTVIEWWIDGET_HPP
#define SCRIPTVIEWWIDGET_HPP

#include <QWidget>
#include<QPainter>
#include <QColor>

class ScriptViewWidget : public QWidget
{
    Q_OBJECT

    int depth, length;

    int brickWidth, brickHeight, textSize;
    void drawBrick;

public:
    explicit ScriptViewWidget(QObject *parent = 0);

signals:

public slots:
    int getBrickWidth();
    int setBirckWidth();

};

#endif // SCRIPTVIEWWIDGET_HPP
