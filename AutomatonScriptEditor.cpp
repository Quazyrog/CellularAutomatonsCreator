#include "AutomatonScriptEditor.hpp"
#include "MainWindow.hpp"



FourScriptButtonsWidget::FourScriptButtonsWidget(QWidget *parent = 0) :
    QWidget(parent)
{
    layout = new QVBoxLayout(this);
    setLayout(layout);

    addNext = new QPushButton(tr("Wstaw kolejny warunek"), this);
    layout->addWidget(addNext);

    addSub = new QPushButton(tr("Wstaw poniżej"), this);
    layout->addWidget(addSub);

    remove = new QPushButton(tr("Usuń element"), this);
    layout->addWidget(remove);

    edit = new QPushButton(tr("Edytuj element"), this);
    layout->addWidget(edit);

}


AutomatonScriptEditor::AutomatonScriptEditor(MainWindow *parent) :
    QDialog(parent)
{
    scriptViewScrollArea = new QScrollArea(this);
    scriptView = new ScriptViewWidget(parent, this);
    scriptViewScrollArea->setWidget(scriptView);
    scriptViewScrollArea->setWidgetResizable(false);
    buttonsWidget = new FourScriptButtonsWidget(this);

    mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    mainLayout->addWidget(scriptViewScrollArea);
    mainLayout->addWidget(buttonsWidget);

    setMinimumSize(600, 400);
    setModal(true);
    setWindowTitle(tr("Konfiguracja reguł przejścia"));

    script = new QList<QString>();
}

