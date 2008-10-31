//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    example.cpp
  \brief   VarTypes QT Example Main File
  \author  Stefan Zickler, (C) 2008
*/

#include <stdio.h>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  //construct QT application
  QApplication app(argc, argv);

  //create mainwindow
  MainWindow mainWin;

  //show mainwindow
  mainWin.show();

  //run QT application's main-loop
  return app.exec();
}
