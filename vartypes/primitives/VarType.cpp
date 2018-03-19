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
  \file    VarType.cpp
  \brief   C++ Implementation: VarType
  \author  Stefan Zickler, (C) 2008
*/

#include "VarType.h"
#include "../gui/VarItemDelegate.h"
#include "VarTypesInstance.h"
#include <VarXML.h>
#include <QApplication>
#include <QMutexLocker>
#include <iostream>
#include <fstream>

namespace VarTypes {
  
  VarType::VarType(string name)
  {
    qRegisterMetaType<VarPtr>("VarPtr");
    #ifndef VDATA_NO_THREAD_SAFETY
    _mutex=new QMutex();
    #endif
    lock();
    _name=name;
    _flags=VARTYPE_FLAG_NONE;
    unlock();
  }
  
  VarType::~VarType()
  {
    #ifndef VDATA_NO_THREAD_SAFETY
    delete _mutex;
    #endif
  }
  
  VarTypeFlag VarType::getFlags() const {
    return _flags;
  }
  
  void VarType::setFlags(VarTypeFlag f) {
    lock();
    VarTypeFlag old=_flags;
    _flags=f;
    if (_flags != old) {
      unlock();
      changed();
    } else {
      unlock();
    }
  }
  
  void VarType::addFlags(VarTypeFlag f) {
    lock();
    VarTypeFlag old=_flags;
    _flags |= f;
    if (_flags != old) {
      unlock();
      changed();
    } else {
      unlock();
    }
  }
  
  void VarType::removeFlags(VarTypeFlag f) {
    lock();
    VarTypeFlag old=_flags;
    _flags = _flags & (~f);
    if (_flags != old) {
      unlock();
      changed();
    } else {
      unlock();
    }
  }
  
  bool VarType::areFlagsSet(VarTypeFlag f) const {
    lock();
    bool answer=(_flags & f) == f;
    unlock();
    return (answer);
  }
  
  
  void VarType::setName(string name)
  {
    lock();
    if (name!=_name) {
      _name=name;
      unlock();
      changed();
    } else {
      unlock();
    }
  }

  string VarType::getTypeDescription()
  {
    return "";
  }

  string VarType::getInstanceDescription()
  {
    QMutexLocker lock(_mutex);
    return _instance_description;
  }

  void VarType::setInstanceDescription(string description)
  {
    QMutexLocker lock(_mutex);
    _instance_description=description;
  }
  
  /// Get the string label of the type of this VarType node.
  string VarType::getTypeName() {
    return VarTypesInstance::getFactory()->stringFromVarType( this->shared_from_this() ) ; 
  } 

  string VarType::getName() const
  {
    return _name;
  }
  
  void VarType::resetToDefault()
  {
    changed();
  }
  
  string VarType::fixString(const char * cst)
  {
    if (cst==0) {
      return "";
    } else {
      return cst;
    }
  }
  
  string VarType::intToString(int val)
  {
    char result[255];
    result[0]=0;
    sprintf(result,"%d",val);
    return result;
  }
  
  string VarType::doubleToString(double val)
  {
    char result[255];
    result[0]=0;
    sprintf(result,"%lf",val);
    return result;
  }
  
  #ifndef VDATA_NO_XML
  XMLNode VarType::findOrAppendChild(XMLNode & parent, string node_name)
  {
    //access ourselves:
    int n=parent.nChildNode(node_name.c_str());
    int myIterator=0;
    //iterate through them and check if we already exist
    if (n>0) {
      XMLNode t=parent.getChildNode(0);
      return t;
    }
    return(parent.addChild(node_name.c_str()));
  }
  
  
  void VarType::updateAttributes(XMLNode & us) const
  {
    (void)us;
  }
  
  void VarType::updateText(XMLNode & us) const
  {
    string tmp;
    getSerialString(tmp);
    us.updateText(tmp.c_str());
  }
  
  void VarType::updateChildren(XMLNode & us, VarTypeImportExportOptions & options) const
  {
    //wipe out all children of type var...
    deleteAllVarChildren(us);
    vector<VarPtr> v = getChildren();
    for (unsigned int i=0;i<v.size();i++) {
      //printf("writing: %d/%d (%s)\n",i,v.size(),v[i]->getName().c_str());
      v[i]->writeXML(us,options);
    }
  }
  
  void VarType::deleteAllVarChildren(XMLNode & node)
  {
    int n=node.nChildNode();
    int i=0;
    for (i=0; i<n; i++) {
      //fixed 08/06/2007: always delete first node in list, instead of using iterator
      XMLNode t=node.getChildNode(0);
      t.deleteNodeContent(1);
    }
  }
  
  void VarType::writeXML(XMLNode & parent, VarTypeImportExportOptions & options)
  {
    if (areFlagsSet(VARTYPE_FLAG_NOSAVE)) return;
    XMLNode us;
   
    //general XML externalization:
      if (options.allow_external_xml_storage && areFlagsSet(VarTypes::VARTYPE_FLAG_EXTERNAL_XML)) {
        us = parent.addChild("ExternalXMLNode");
        std::string ext_name = getExternalXMLFilename();
        std::string external_filename = options.getUniqueName(getExternalXMLFilename(), ".xml", true);
        us.updateText(external_filename.c_str());
        //temporarily remove flag
        removeFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
        //run external storage...
        //check if path is absolute... if not, make it relative to root xml.
        VarTypeImportExportOptions options_external = options;
  
        QFileInfo fileinfo(QString::fromStdString(external_filename));
        if (fileinfo.isAbsolute()==false) {
          external_filename =  options.getBasePath() + QDir::separator().toLatin1() + external_filename;
        }
        VarXML::write(shared_from_this(), external_filename, options_external);
        //re-instate flag
        addFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
        Q_EMIT(XMLwasWritten(this->shared_from_this()));
        return;
      } else if (options.use_external_blob_output && areFlagsSet(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY)) { //if true, then serialized text content will be written in its own file with unique name and type-dependent, preferred extension.
      us = parent.addChild("ExternalBlob");
      us.updateAttribute(getName().c_str(),"name","name");
      us.updateAttribute(getTypeName().c_str(),"type","type");
//     } else if (options.use_external_xml_output) { //future feature
//       us = parent.addChild("ExternalXML");
    } else {
      //printf("blind append is: %d\n", options.blind_append ? 1: 0);
      if (options.blind_append) {
        //do a simple append to the XML parent
        us = parent.addChild(getTypeName().c_str());
      } else {
        //do a name-based find and update, or append if it does not exist.
        us = findOrAppendChild(parent, getTypeName().c_str());
      }
      us.updateAttribute(getName().c_str(),"name","name");
    }
    updateAttributes(us);
    
    //us.updateAttribute(getTypeName().c_str(),"type","type");

    if (options.use_external_blob_output && areFlagsSet(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY)) {
      std::string filename = options.getUniqueName(getName(), getSerializedContentsFilenameExtension());
      std::string filename_full = options.getBasePath() + QDir::separator().toLatin1() + filename;
      std::ofstream outfile(filename_full.c_str());
      serializeContentsToStream(outfile); 
      outfile.close();
      us.updateText(filename.c_str());
    } else {
      updateText(us);
    }
    
    updateChildren(us, options);
    Q_EMIT(XMLwasWritten(this->shared_from_this()));
  }
  
  
  void VarType::readAttributes(XMLNode & us)
  {
    (void)us;
  }
  
  void VarType::readText(XMLNode & us)
  {
    setSerialString(fixString(us.getText()));
  }
  
  void VarType::readChildren(XMLNode & us, VarTypeImportExportOptions & options)
  {
    (void)us;
  }
  
  void VarType::readXML(XMLNode & us, VarTypeImportExportOptions & options, bool is_external)
  {
    if (areFlagsSet(VARTYPE_FLAG_NOLOAD)) return;
    //printf("readXML: %s\n",this->getName().c_str());
    if (areFlagsSet(VARTYPE_FLAG_NOLOAD_ATTRIBUTES)==false) readAttributes(us);
    
    if (is_external) {
      std::string filename = fixString(us.getText());
      filename = options.getBasePath() + QDir::separator().toLatin1() + filename;
      ifstream infile(filename.c_str());
      serializeContentsFromStream(infile);
      infile.close();
    } else {
      readText(us);
    }
    
    readChildren(us, options);
    Q_EMIT(XMLwasRead(this->shared_from_this()));
  }
  
  void VarType::loadExternal(VarTypeImportExportOptions & options) {
  
  }
  
  
    vector<VarPtr> VarType::readChildrenHelper(XMLNode & parent , vector<VarPtr> existing_children, bool only_update_existing, bool blind_append, VarTypeImportExportOptions & options)
  {
    //this again does non-destructive integration
    //As a result we update any predefined structures and
    //append any types that don't exist yet.
    vector<VarPtr> result=existing_children;
    int n=parent.nChildNode();
    int i,myIterator=0;

    //iterate through them and check if we already exist
    set<VarPtr> unmatched_children;
    for (unsigned int j=0;j<existing_children.size();j++) {
      unmatched_children.insert(existing_children[j]);
    }
    for (i=0; i<n; i++) {
      XMLNode t=parent.getChildNode(myIterator);
      myIterator++;
      string sname=fixString(t.getAttribute("name"));
      string stype=fixString(t.getName());
      //VarTypeId type=VarTypesInstance::getFactory()->stringToTypeInfo(stype);
      bool is_external_blob=false;
      bool is_external_xml_node=false;
//       VarPtr imported_ptr;
      VarTypeImportExportOptions external_options;
      std::string filename="";
      if (stype=="ExternalBlob") {
        //special treatment!
        stype=fixString(t.getAttribute("type"));
        is_external_blob=true;
      } else if (stype=="ExternalXMLNode") {
        is_external_xml_node=true;
        //TODO FIXME
        //if (blind_append==false) prenodes.push_back(parent_vartype);
        //TO REALLY FIX THIS, WE NEED A TWO-PASS APROACH:
        //1. read raw xml using XMLNode parent = XMLNode::openFileHelper(filename.c_str(),"VarXML");
        //2. parse the type and name of the root node in the external xml file
        //3. search that node in the existing varchildren
        //4. add that node as prenodes and run VarXML::read...
        filename = fixString(t.getText());
        QFileInfo fileinfo(QString::fromStdString(filename));
        std::string full_filename = filename;
        if (fileinfo.isAbsolute()==false) {
          full_filename = options.getBasePath() + QDir::separator().toLatin1() + filename;
        }
        
        XMLNode import_root = XMLNode::openFileHelper(full_filename.c_str(),"VarXML");
        int xmlnc = import_root.nChildNode();
        //vector<VarPtr> imported = VarXML::read(prenodes, full_filename, options);
        if (xmlnc > 1) {
          fprintf(stderr, "WARNING: When importing node from file: '%s': XML file contains more than 1 root node! Using only first node.\n", filename.c_str());
        }
        //printf("import root name: %s\n", import_root.getName());
        sname="";
        stype="";
        if (xmlnc > 0) {
          t=import_root.getChildNode(0);
          sname=fixString(t.getAttribute("name"));
          stype=fixString(t.getName());
          //printf("import name: %s and type: %s\n", sname.c_str(), stype.c_str());
          external_options=options;
          external_options.setBasename(full_filename);

        } else {
          continue; 
        }
      }
      if (VarTypesInstance::getFactory()->isRegisteredType(stype)) {
        bool found=false;
        if (sname!="" && blind_append==false) {
          //try to find existing child with this name
          for (unsigned int j=0;j<existing_children.size();j++) {
            if (existing_children[j]->getName()==sname) {
              if (existing_children[j]->getTypeName()==stype) {
                  existing_children[j]->readXML(t, is_external_xml_node ? external_options : options, is_external_blob);
                  if (is_external_blob) {
                    existing_children[j]->addFlags(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
                  } else {
                    existing_children[j]->removeFlags(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
                  }
                  if (is_external_xml_node) {
                    options.errors_found = options.errors_found||external_options.errors_found;
                    existing_children[j]->addFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
                    existing_children[j]->setExternalXMLFilename(filename);
                  } else {
                    existing_children[j]->removeFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
                    existing_children[j]->setExternalXMLFilename("");                   
                  }
              } else {
                fprintf(stderr,"Type mismatch between XML and Data-Tree. Object name: %s. XML type was: %s. Internal type was: %s\n",sname.c_str(),stype.c_str(),existing_children[j]->getTypeName().c_str());
              }
              unmatched_children.erase(existing_children[j]);
              found=true;
              break;
              //ok matching name was found...update this node
            }
          }
        }
        if (found==false) {
          //printf("unable to find: %s in existing\n",sname.c_str());
          //the item did not already exist in the children...
          if (only_update_existing==false) {
            //create new node and append it:
//             if (is_external_xml_node) {
//               //it already exists...
//               result.push_back(imported_ptr);
//             } else {
              VarPtr td= VarTypesInstance::getFactory()->newVarType(stype);
              td->setName(sname);
              if (is_external_blob) {
                td->addFlags(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
              } else {
                td->removeFlags(VarTypes::VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
              }
              if (is_external_xml_node) {
                td->addFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
                td->setExternalXMLFilename(filename);
              } else {
                td->removeFlags(VarTypes::VARTYPE_FLAG_EXTERNAL_XML);
                td->setExternalXMLFilename("");                   
              }
              td->readXML(t, is_external_xml_node ? external_options : options, is_external_blob);
              if (is_external_xml_node) {
                options.errors_found = options.errors_found||external_options.errors_found;
              }
              result.push_back(td);
            //}
          }
  
        }
      } else {
        fprintf(stderr,"Error: Found node of unknown type in XML. Type: '%s'\n",stype.c_str());
        options.errors_found=true;
      }
    }
    //---fix for recursing tree that's not defined in xml yet:
    queue<VarPtr> _queue;
    for (set<VarPtr>::iterator iter = unmatched_children.begin(); iter!=unmatched_children.end(); iter++) {
      if (*iter!=0) _queue.push(*iter);
    }
    //recurse all unmatched children to make sure we load all externals:
    while(_queue.empty()==false) {
      VarPtr d = _queue.front();
      _queue.pop();
      d->loadExternal(options);
      vector<VarPtr> children = d->getChildren();
      int s=children.size();
      for (int i=0;i<s;i++) {
        if (children[i]!=0) _queue.push(children[i]);
      }
    }
    //---end of fix
    return result;
  }
  
  //------------MODEL VIEW GUI STUFF:
  void VarType::paint (const VarItemDelegate * delegate, QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    //let the QT default delegate do the painting:
    delegate->QItemDelegate::paint(painter,option,index);
  }
  
  QWidget * VarType::createEditor(const VarItemDelegate * delegate, QWidget *parent, const QStyleOptionViewItem &option) {
    (void)delegate;
    (void)option;
    return new QLineEdit(parent);
  }
  
  void VarType::setEditorData(const VarItemDelegate * delegate, QWidget *editor) const {
    (void)delegate;
    QLineEdit * ledit=(QLineEdit *) editor;
    ledit->setText(QString::fromStdString(getString()));
  }
  
  void VarType::setModelData(const VarItemDelegate * delegate, QWidget *editor) {
    (void)delegate;
    QLineEdit * ledit=(QLineEdit *) editor;
    if (setString(ledit->text().toStdString())) mvcEditCompleted();
  }
  
  QSize VarType::sizeHint(const VarItemDelegate * delegate, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    return (delegate->QItemDelegate::sizeHint(option, index));
  }
};
#endif
