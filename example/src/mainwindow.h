//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    mainwindow.h
  \brief   VarTypes QT Example MainWindow Header
  \author  Stefan Zickler, (C) 2008
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QSplitter>
#include <QStringList>
#include <QMainWindow>

#include "VarTreeModel.h"
#include "VarItem.h"
#include "VarTreeView.h"
#include "VarXML.h"
#include "VarTypes.h"
using namespace VarTypes;
class MainWindow : public QMainWindow
{
  Q_OBJECT

protected:
  vector<VarPtr> world; // our list of toplevel node(s)

  VarTreeView  * tree_view; // the GUI
  VarTreeModel * tmodel;    // the GUI's underlying datamodel

public:
  MainWindow();
  virtual ~MainWindow();

  virtual void closeEvent(QCloseEvent * event );

public slots:
  void notificationExample(VarPtr node);
};


#endif
