#include "MainWindow.hpp"
#include "CellularAutomaton.hpp"
#include <QApplication>

#include <string>
#include <csignal>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <QTextCodec>
#include <QtGlobal>
#include "Exceptions.hpp"


void halt(int sig)
{
    std::cerr << "Unexpected signal " << sig << ". Aborting." << std::endl;
    std::cerr.flush();
    abort();
}


int main(int argc, char *argv[])
{
    CellularAutomaton::initialize();
    signal(SIGSEGV, halt);
    srand(time(NULL));

#if QT_VERSION <= 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
