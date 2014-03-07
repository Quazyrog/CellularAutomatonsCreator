#include "MainWindow.hpp"
#include <QApplication>

#include <csignal>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QTextCodec>
#include <QtGlobal>

void halt(int sig)
{
    std::cerr << "Unexpected signel " << sig << ". Aborting." << std::endl;
    std::cerr.flush();
    abort();
}


int main(int argc, char *argv[])
{
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
