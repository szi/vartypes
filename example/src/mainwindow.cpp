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
  \file    mainwindow.h
  \brief   VarTypes QT Example MainWindow Implementation
  \author  Stefan Zickler, (C) 2008
*/

#include "mainwindow.h"

MainWindow::MainWindow()
{
  tree_view=new VarTreeView(); //construct the GUI widget:
  tmodel=new VarTreeModel(); //construct the GUI widget's datamodel:
  tree_view->setModel(tmodel); //link the two together

  //=====================================================
  //=======BEGIN OF DATA-TREE CONSTRUCTION===============
  //=====================================================

  //instanciate the root:
  root = new VarList("root node");

  //add a sub-node:
  VarList * v_list1 = new VarList("a subtree");
  root->addChild(v_list1); //let it be a child of the root

  //create a trigger-button
  VarTrigger * v_trigger1 = new VarTrigger("trigger","click me");
  v_list1->addChild(v_trigger1); //add it to the tree

  //create a variable of type double:
  VarDouble * v_double1 = new VarDouble("my double");
  v_list1->addChild(v_double1); //add it to the tree

  //create another variable of type double, with a default value:
  VarDouble * v_double2 = new VarDouble("double w/ default",20.0);
  v_list1->addChild(v_double2);  //add it to the tree

  VarDouble * v_double3 = new VarDouble("double w/ persistent editor");
  v_double3->addRenderFlags(DT_FLAG_PERSISTENT);
  v_list1->addChild(v_double3); //add it to the tree

  //create another variable of type double, with a default value
  //and min/max ranges
  VarDouble * v_double4 = new VarDouble("double with min/max",0.5,0.0,1.0);
  v_list1->addChild(v_double4);  //add it to the tree

  VarList * v_list2 = new VarList("another subtree");
  root->addChild(v_list2);

  v_list2->addChild(new VarString("a string"));

  v_list2->addChild(new VarString("string w/ default","foobar"));

  VarList * v_list3 = new VarList("a sub-sub tree");
  v_list2->addChild(v_list3);

  v_list3->addChild(new VarInt("an integer"));
  v_list3->addChild(new VarBool("a boolean"));

  VarStringEnum * v_string_enum = new VarStringEnum("food-selector","French Fries");
  v_string_enum->addRenderFlags(DT_FLAG_PERSISTENT);
  v_string_enum->addItem("Cheeseburger");
  v_string_enum->addItem("French Fries");
  v_string_enum->addItem("Ice Cream");

  v_list1->addChild(v_string_enum);

  //some example access
  printf("old value of node '%s': %f\n",v_double1->getName().c_str(),v_double1->getDouble());

  //some example mutation
  v_double1->setDouble(30.0);

  printf("new value of node '%s': %f\n",v_double1->getName().c_str(),v_double1->getDouble());

  //instead of using accessors/mutators you can also wait for user-input
  //by using QT event-handling
  //in this example we link a node to a local event function
  //Note that each vartype has two events:
  // "hasChanged()" will be triggered whenever the internal data changes,
  // no matter whether this happened programmatically or through the GUI
  // "wasEdited(VarData *)" will be triggered only if the data was changed by the user
  // through the GUI. This is usually what you want.

  //For this event-example, first let's create a standard node:
  VarDouble * v_double = new VarDouble("Event Example (change my value and see console!)");
  root->addChild(v_double);

  //now let's hook up the wasEdited event to a local function 'notificationExample'
  //which is defined further below in this file
  //we do so using QT's 'connect' function
  //qt will make sure that everytime a user changes the node, the event will fire.
  connect(v_double,SIGNAL(wasEdited(VarData *)),this,SLOT(notificationExample(VarData *)));

  //finally an example of a subtree that's stored in a separate XML file:
  //simply use VarExternal instead of VarList and provide a filename as constructor.
  //that's it!
  VarExternal * v_ext = new VarExternal("other_settings.xml","this tree has its own XML file");
  root->addChild(v_ext);
  v_ext->addChild(new VarInt("another int"));
  v_ext->addChild(new VarBool("a bool value defaulting to true",true));

  //=====================================================
  //=======END OF DATA-TREE CONSTRUCTION=================
  //=====================================================

  //the root is our only toplevel node
  //(you can have multiple roots if desired):
  world.push_back(root);

  //read settings from XML file
  world=VarXML::read(world,"settings.xml");

  //tell our gui what toplevel node(s) we want to visualize
  tmodel->setRootItems(world);

  //optional: expand the root node:
  //Hint: you can call this on any node:
  tree_view->expandAndFocus(root);

  //optional: fit columns to maximum data-width:
  tree_view->fitColumns();

  //make the tree view the application's one and only widget:
  setCentralWidget(tree_view);

  //give this window a decent size
  resize(640,480);
}

void MainWindow::notificationExample(VarData * node) {
  printf("Value of node '%s' was changed to: '%s'\n",node->getName().c_str(),node->getString().c_str());
}

void MainWindow::closeEvent(QCloseEvent * event ) {
  (void)event;
  //delete this;
}

MainWindow::~MainWindow() {
  //write out the XML
  VarXML::write(world,"settings.xml");

  //and exit
  exit(0);
}
