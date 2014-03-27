#include "AutomatonScriptEditor.hpp"
#include "MainWindow.hpp"



ScriptButtonsWidget::ScriptButtonsWidget(AutomatonScriptEditor *parent) :
    QWidget(parent)
{
    layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setAlignment(Qt::AlignTop);

    addNext = new QPushButton(tr("Wstaw kolejny warunek"), this);
    layout->addWidget(addNext);

    addSub = new QPushButton(tr("Wstaw poniżej"), this);
    layout->addWidget(addSub);

    remove = new QPushButton(tr("Usuń element"), this);
    layout->addWidget(remove);

    edit = new QPushButton(tr("Edytuj element"), this);
    layout->addWidget(edit);

    layout->addSpacing(30);

    setDefault = new QPushButton(tr("Edytuj wartość domyślną"), this);
    layout->addWidget(setDefault);

    installScript = new QPushButton(tr("Zastosuj"), this);
    layout->addWidget(installScript);
    layout->setAlignment(installScript, Qt::AlignBottom);

    connect(addNext, SIGNAL(clicked()), parent->scriptView, SLOT(addAfterSelected()));
    connect(addSub, SIGNAL(clicked()), parent->scriptView, SLOT(addUnderSelected()));
    connect(remove, SIGNAL(clicked()), parent->scriptView, SLOT(eraseSelected()));
    connect(edit, SIGNAL(clicked()), parent->scriptView, SLOT(editSelected()));
    connect(setDefault, SIGNAL(clicked()), parent->scriptView, SLOT(editDefault()));
    connect(installScript, SIGNAL(clicked()), parent, SLOT(installScript()));
}


AutomatonScriptEditor::AutomatonScriptEditor(MainWindow *parent) :
    QDialog(parent)
{
    scriptViewScrollArea = new QScrollArea(this);
    scriptView = new ScriptViewWidget(parent, scriptViewScrollArea);
    scriptViewScrollArea->setWidget(scriptView);
    scriptViewScrollArea->setWidgetResizable(false);
    buttonsWidget = new ScriptButtonsWidget(this);

    mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    mainLayout->addWidget(scriptViewScrollArea);
    mainLayout->addWidget(buttonsWidget);

    mainWindow = parent;

    setMinimumSize(600, 400);
    setModal(true);
    setWindowTitle(tr("Konfiguracja reguł przejścia"));
}


void AutomatonScriptEditor::installScript()
{
    mainWindow->world->setScript(scriptView->compile());
    hide();
}