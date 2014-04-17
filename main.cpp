/* Copyright 2014 Wojciech Matusiak
 *
 * This file is part of CellularAutomatonCreator.
 *
 * CellularAutomatonCreator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CellularAutomatonCreator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CellularAutomatonCreator.  If not, see <http://www.gnu.org/licenses/>.
 */


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
