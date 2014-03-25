#include "ScriptViewWidget.hpp"

ScriptViewWidget::ScriptViewWidget(QObject *parent) :
    QWidget(parent)
{
    depth = 0;
    width = 0;

    brickHeight = 30;
    brickWidth = 300;
    textSize = 20;
}
