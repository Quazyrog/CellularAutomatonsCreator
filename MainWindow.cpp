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

    _simulationTimer = new QTimer(this);
    _simulationTimer->setInterval(500);
    connect(_simulationTimer, SIGNAL(timeout()), this, SLOT(nextGenerationHandler()));
    connect(_simulationTimer, SIGNAL(timeout()), _gridViewer, SLOT(update()));

    createFileMenu();
    createSimulationMenu();
    createGridPaintingMenu();

    QSettings settings(".cacsettings", QSettings::NativeFormat);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    setWindowIcon(QIcon(":/ico/Icon.png"));
    statusBar()->showMessage("Done");
}


void MainWindow::createFileMenu()
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
    connect(_fileSaveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    _fileSaveAsAction = new QAction(QIcon::fromTheme("document-save-as"), "Save as", this);
    _fileSaveAsAction->setShortcut(QKeySequence::SaveAs);
    _fileMenu->addAction(_fileSaveAsAction);
    _fileMenu->addSeparator();
    connect(_fileSaveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    _fileCloseAction = new QAction(QIcon::fromTheme("application-exit"), "Quit", this);
    _fileCloseAction->setShortcut(QKeySequence::Quit);
    _fileMenu->addAction(_fileCloseAction);
    connect(_fileCloseAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}


void MainWindow::createSimulationMenu() {
    _simulationMenu = new QMenu("Simulation", this);
    _simulationTogglePause = new QAction(QIcon::fromTheme("media-playback-pause"), "Toggle pause", this);
    _simulationMenu->addAction(_simulationTogglePause);
    _simulationTogglePause->setShortcut(Qt::Key_Space);
    connect(_simulationTogglePause, SIGNAL(triggered()), this, SLOT(togglePause()));

    menuBar()->addMenu(_simulationMenu);
    _simulationStopAction = new QAction(QIcon::fromTheme("media-playback-stop"), "Stop simulation", this);
    _simulationMenu->addAction(_simulationStopAction);
    connect(_simulationStopAction, SIGNAL(triggered()), _simulationTimer, SLOT(stop()));

    _simulationResumeAction = new QAction(QIcon::fromTheme("media-playback-start"), "Start simulation", this);
    _simulationMenu->addAction(_simulationResumeAction);
    connect(_simulationResumeAction, SIGNAL(triggered()), _simulationTimer, SLOT(start()));


    _simulationMenu->addSeparator();
    _simulationSpeedSubmenu = new QMenu("Simulation speed", this);
    _simulationMenu->addMenu(_simulationSpeedSubmenu);

    _simulationSpeed1000 = new QAction("Slow (1000ms delay)", this);
    _simulationSpeedSubmenu->addAction(_simulationSpeed1000);
    connect(_simulationSpeed1000, &QAction::triggered, [this](){ _simulationTimer->setInterval(1000); });

    _simulationSpeed500 = new QAction("Normal (500ms delay)", this);
    _simulationSpeedSubmenu->addAction(_simulationSpeed500);
    connect(_simulationSpeed500, &QAction::triggered, [this](){ _simulationTimer->setInterval(500); });

    _simulationSpeed333 = new QAction("Fast (333ms delay)", this);
    _simulationSpeedSubmenu->addAction(_simulationSpeed333);
    connect(_simulationSpeed333, &QAction::triggered, [this](){ _simulationTimer->setInterval(333); });

    _simulationSpeed250 = new QAction("Very fast (250ms delay)", this);
    _simulationSpeedSubmenu->addAction(_simulationSpeed250);
    connect(_simulationSpeed250, &QAction::triggered, [this](){ _simulationTimer->setInterval(250); });
}


void MainWindow::createGridPaintingMenu()
{
    _gridPaintingMenu = new QMenu("Grid painting", this);
    menuBar()->addMenu(_gridPaintingMenu);

    _fillGridWithZeros = new QAction("Fill grid with zeros", this);
    _gridPaintingMenu->addAction(_fillGridWithZeros);
    connect(_fillGridWithZeros, &QAction::triggered, [this](){
        _simulationTimer->stop();
        _generationCounter = 0;

        if (_automaton != nullptr)
                _automaton->fillGrid();
        _gridViewer->update();
    });

    _gridPaintingMenu->addSeparator();
    _resizeGridAction = new QAction("Resize grid", this);
    _gridPaintingMenu->addAction(_resizeGridAction);
    connect(_resizeGridAction, SIGNAL(triggered()), this, SLOT(showResizeDialog()));
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
    _simulationTimer->stop();

    QFileDialog fileDialog;
    fileDialog.setWindowTitle("Open file");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    QString selectedFile;
    if (!fileDialog.exec())
        return;
    selectedFile = fileDialog.selectedFiles()[0];

    try {
        setAutomaton(Scripting::CellularAutomaton::readFromFile(selectedFile));
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
        msgbox.setText("Failed to open file – io error.");
        msgbox.exec();
        setStatusTip("Failed to open file " + selectedFile);
        return;
    }

    _documentPath = selectedFile;
    statusBar()->showMessage("Opened file " + selectedFile);
}


void MainWindow::saveFile()
{
    if (_documentPath == QString("")) {
        saveFileAs();
        return;
    }

    try {
        _automaton->saveToFile(_documentPath);
    } catch (Exceptions::IOException &except) {
        qWarning() << "Saving file failed:" << except.what();
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Failed to save file");
        msgbox.setText("Failed to save file – io error.");
        msgbox.exec();
        setStatusTip("Failed to open file " + _documentPath);
    }

    qDebug() << "Saved " << _documentPath;
    statusBar()->showMessage("Saved file " + _documentPath);
}


void MainWindow::saveFileAs()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle("Save file");
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    if (_documentPath != QString(""))
        fileDialog.setDirectory(QFileInfo(_documentPath).dir());

    QString selectedFile;
    if (!fileDialog.exec())
        return;
    selectedFile = fileDialog.selectedFiles()[0];

    try {
        _automaton->saveToFile(selectedFile);
    } catch (Exceptions::IOException &except) {
        qWarning() << "Saving file failed:" << except.what();
        QMessageBox msgbox;
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle("Failed to save file");
        msgbox.setText("Failed to save file – io error.");
        msgbox.exec();
        setStatusTip("Failed to open file " + selectedFile);
        return;
    }

    _documentPath = selectedFile;

    qDebug() << "Saved " << _documentPath;
    statusBar()->showMessage("Saved file " + selectedFile);
}


void MainWindow::setAutomaton(Scripting::CellularAutomaton *automaton)
{
    if (automaton == nullptr) {
        qWarning() << "Attempt to set _automaton to _nullptr!";
        return;
    }

    _simulationTimer->stop();
    _generationCounter = 0;
    _gridViewer->setDisplayedAutomaton(automaton);
    _stateSwitch->setAutomaton(automaton);
    if (_automaton != nullptr) {
        _automaton->disconnect(_simulationTimer, SIGNAL(timeout()));
        delete _automaton;
    }

    _automaton = automaton;
    connect(_simulationTimer, SIGNAL(timeout()), _automaton, SLOT(nextGeneration()));
}


void MainWindow::nextGenerationHandler()
{
    ++_generationCounter;
    statusBar()->showMessage(QString().sprintf("Generation: %u", _generationCounter));
}


void MainWindow::showResizeDialog()
{

    _simulationTimer->stop();

    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(10);
    dialog.setIntMaximum(200);

    int w, h;
    dialog.setLabelText("Width:");
    if (dialog.exec() == QDialog::Rejected)
        return;
    w = dialog.intValue();

    dialog.setLabelText("Height:");
    if (dialog.exec() == QDialog::Rejected)
        return;
    h = dialog.intValue();

    try {
        _automaton->resizeGrid(w, h);
    } catch (Exceptions::IndexOutOfBoundsException) {
        QMessageBox(QMessageBox::Critical, "Invalid size", "Grid is too large. Number of cells must not be greater than " + QString::number(Scripting::CellularAutomaton::GRID_SIZE_LIMIT) + ".");
    } catch (Exceptions::IllegalArgumentException) {
        QMessageBox(QMessageBox::Critical, "Invalid size", "Invalid size, but why?");
    }

    _generationCounter = 0;
}


void MainWindow::togglePause()
{
    if (_simulationTimer->isActive())
        _simulationTimer->stop();
    else
        _simulationTimer->start();
}
