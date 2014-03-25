#ifndef AUTOMATONSCRIPTEDITOR_HPP
#define AUTOMATONSCRIPTEDITOR_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QPushButton>

class MainWindow;


class FourScriptButtonsWidget : public QWidget
{
    Q_OBJECT

    QVBoxLayout *layout;
    QPushButton *addNext, *addSub, *remove, *edit;
public:
    FourScriptButtonsWidget(QWidget *parent);
};


class AutomatonScriptEditor : public QDialog
{
    Q_OBJECT

    QHBoxLayout *mainLayout;
    FourScriptButtonsWidget *buttonsWidget;

    QTreeView *scriptView;
    QList<QString> *script;

public:
    AutomatonScriptEditor(MainWindow *parent);
};

#endif // AUTOMATONSCRIPTEDITOR_HPP
