#ifndef AUTOMATONSCRIPTEDITOR_HPP
#define AUTOMATONSCRIPTEDITOR_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

#include "ScriptViewWidget.hpp"

class MainWindow;
class AutomatonScriptEditor;


class ScriptButtonsWidget : public QWidget
{
    Q_OBJECT

    QVBoxLayout *layout;
    QPushButton *addNext, *addSub, *remove, *edit, *setDefault, *installScript;
public:
    ScriptButtonsWidget(AutomatonScriptEditor *parent);
};


class AutomatonScriptEditor : public QDialog
{
    Q_OBJECT

    friend class ScriptButtonsWidget;

    QHBoxLayout *mainLayout;
    ScriptButtonsWidget *buttonsWidget;

    ScriptViewWidget *scriptView;
    QScrollArea *scriptViewScrollArea;
    QList<QString> *script;

    MainWindow *mainWindow;

public:
    AutomatonScriptEditor(MainWindow *parent);

public slots:
    void installScript();
};

#endif // AUTOMATONSCRIPTEDITOR_HPP
