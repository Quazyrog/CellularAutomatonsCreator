/* Copyright 2014, 2015 Wojciech Matusiak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.hpp"

#include "GridViewer.hpp"
#include "CellularAutomaton.hpp"
#include "StateSwitchWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _automaton = nullptr;

    _gridViewer = new GridViewer(this);
    setCentralWidget(_gridViewer);

    _stateSwitch = new StateSwitchDock(_gridViewer, this);
    addDockWidget(Qt::LeftDockWidgetArea, _stateSwitch);

    _statusBar = new QStatusBar(this);
    setStatusBar(_statusBar);

    createMenuBar();

    QSettings settings(".cacsettings", QSettings::NativeFormat);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    setWindowIcon(QIcon(":/ico/Icon.png"));
    setStatusTip("Done");
}


void MainWindow::createMenuBar()
{
    _fileMenu = new QMenu("File", this);
    menuBar()->addMenu(_fileMenu);
    _fileNewAction = new QAction(QIcon::fromTheme("document-new"), "New", this);
    _fileNewAction->setShortcut(QKeySequence::New);
    _fileMenu->addAction(_fileNewAction);
    _fileOpenAction = new QAction(QIcon::fromTheme("document-open"), "Open", this);
    _fileOpenAction->setShortcut(QKeySequence::Open);
    _fileMenu->addAction(_fileOpenAction);
    connect(_fileOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));
    _fileMenu->addSeparator();
    _fileSaveAction = new QAction(QIcon::fromTheme("document-save"), "Save", this);
    _fileSaveAction->setShortcut(QKeySequence::Save);
    _fileMenu->addAction(_fileSaveAction);
    _fileSaveAsAction = new QAction(QIcon::fromTheme("document-save-as"), "Save as", this);
    _fileSaveAsAction->setShortcut(QKeySequence::SaveAs);
    _fileMenu->addAction(_fileSaveAsAction);
    _fileMenu->addSeparator();
    _fileCloseAction = new QAction(QIcon::fromTheme("application-exit"), "Quit", this);
    _fileCloseAction->setShortcut(QKeySequence::Quit);
    _fileMenu->addAction(_fileCloseAction);
    connect(_fileCloseAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}


MainWindow::~MainWindow()
{
    delete _automaton;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(".cacsettings", QSettings::NativeFormat);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
}


void MainWindow::openFile()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle("Open file");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    QString selectedFile;
    if (!fileDialog.exec())
        return;
    selectedFile = fileDialog.selectedFiles()[0];

    try {
        Scripting::CellularAutomaton *newAutomaton;
        newAutomaton = Scripting::CellularAutomaton::readFromFile(selectedFile);
        _gridViewer->setDisplayedAutomaton(newAutomaton);
        _stateSwitch->setAutomaton(newAutomaton);
        if (_automaton != nullptr)
            delete _automaton;
        _automaton = newAutomaton;
    } catch (Exceptions::SyntaxErrorException &except) {
        qWarning() << "Opening file failed:" << except.what() << "–" << except.message() << "at" << except.where();
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Failed to open file");
        msgbox.setText("Failed to open file <i>" + selectedFile + "</i>. Automaton said, that line " + QString::number(except.where()) + " is invalid – <i>" + except.message() + QString("</i>."));
        msgbox.exec();
        setStatusTip("Failed to open file " + selectedFile);
        return;
    } catch (Exceptions::IOException &except) {
        qWarning() << "Opening file failed:" << except.what();
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Failed to open file");
        msgbox.setText("Failed to open file – illegal argument exception.");
        msgbox.exec();
        setStatusTip("Failed to open file " + selectedFile);
        return;
    }

    setStatusTip("Opened file " + selectedFile);
}
