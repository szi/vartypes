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
  \file    VarType.h
  \brief   C++ Interface: VarType
  \author  Stefan Zickler, (C) 2008
*/
#ifndef VDATATYPE_H_
#define VDATATYPE_H_
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <limits.h>
#include <float.h>
#include <set>
#include <queue>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QLineEdit>
#include "VarVal.h"
#include <QObject>
#include <iostream>
  #ifndef VDATA_NO_XML
    #include "xmlParser.h"
  #endif
  
  #ifndef VDATA_NO_THREAD_SAFETY
    #include <QMutex>
  #endif
#include <QFileInfo>
#include <QDir>

//Would use QSharedPointer instead of the tr1/boost versions, but enable_shared_from_this is not yet implemented in Qt which is a functionality that we need as we have vartypes generating events with pointers to themselves.
#include <memory>
//#include <bits/algorithmfwd.h>
#include <QApplication>


namespace VarTypes {
  
  class VarTypeImportExportOptions {
  protected:
    std::set<std::string> used_filenames;
    QFileInfo basename;
    //int unique_count;
  public:
    bool use_external_blob_output;
    bool blind_append;
    bool allow_external_xml_storage;
    bool errors_found;
    bool abort_on_error;
    VarTypeImportExportOptions() {
      use_external_blob_output=false;
      used_filenames.clear();
      blind_append=false;
      allow_external_xml_storage=true;
      errors_found=false;
      //unique_count=0;
    }
    std::string getBasePath() {
      return basename.path().toStdString();
    }
    void setBasename(std::string str_basename) {
      basename = QFileInfo(QString::fromStdString(str_basename));
      used_filenames.insert(basename.fileName().toStdString());
    }
    //std::string getBasename();
    std::string getUniqueName(std::string desired_name="", std::string desired_extension=".bin", bool use_verbatim_desired_filepath=false) {
      if (desired_name=="") {
        desired_name="external";
      }
      if (use_verbatim_desired_filepath && used_filenames.find(desired_name)==used_filenames.end()) {
        used_filenames.insert(desired_name);
        return desired_name;
      }
      
      int unique_adder=0;
      while(true) {
        QFileInfo desired_info(QString::fromStdString(desired_name));
        QString desired_base = desired_info.baseName();
        if (use_verbatim_desired_filepath) {
          desired_name = (desired_info.path() + QDir::separator() + desired_base).toStdString();
        }
        std::string root_name = basename.baseName().toStdString();
        char cbuf[255];
        sprintf(cbuf,".%d",unique_adder);
        std::string adder_string="";
        if (unique_adder>0) {
          adder_string = std::string(".") + cbuf;
        }
        std::string fname;
        if (use_verbatim_desired_filepath) {
          fname = desired_name + adder_string + desired_extension;
        } else {
          fname = root_name + "_" + desired_name + adder_string + desired_extension;
        }
        if (used_filenames.find(fname)==used_filenames.end()) {
          used_filenames.insert(fname);
          return fname;
        }
        unique_adder++;
      }
    }
  };
  
  class VarTypeSearchOptions {
    protected:
      int flags;
      unsigned int max_results;
    public:
      enum VarTypeSearchFlags {
        SEARCH_FLAG_DEFAULT          = 0x00,
        SEARCH_FLAG_IGNORE_CASE      = 0x01,
        SEARCH_FLAG_SEARCH_SUBTREES  = 0x01 << 1,
        SEARCH_FLAG_ENABLE_DEBUG     = 0x01 << 2,
      };
      VarTypeSearchOptions(bool ignore_case=false, bool search_subtree=false) {
        clear();
        setIgnoreCase(ignore_case);
        setSearchSubtrees(search_subtree);

      }
      VarTypeSearchOptions(int search_flags) {
        clear();
        flags=search_flags;
      }
      int getMaximumNumberOfResults() const {
        return max_results;
      }
      void setMaximumNumberOfResults(int max_result_count) {
        max_results=max_result_count;
      }
      bool checkIgnoreCase() const {
        return ((flags & SEARCH_FLAG_IGNORE_CASE) == SEARCH_FLAG_IGNORE_CASE);
      }
      bool checkSearchSubtrees() const {
        return ((flags & SEARCH_FLAG_SEARCH_SUBTREES) == SEARCH_FLAG_SEARCH_SUBTREES);
      }
      bool checkEnableDebug() const {
        return ((flags & SEARCH_FLAG_ENABLE_DEBUG) == SEARCH_FLAG_ENABLE_DEBUG);
      }
      void setEnableDebug(bool value) {
        if (value) {
          flags|=SEARCH_FLAG_ENABLE_DEBUG;
        } else {
          flags&=(~SEARCH_FLAG_ENABLE_DEBUG);
        }
      }
      void setIgnoreCase(bool value) {
        if (value) {
          flags|=SEARCH_FLAG_IGNORE_CASE;
        } else {
          flags&=(~SEARCH_FLAG_IGNORE_CASE);
        }
      }
      void setSearchSubtrees(bool value) {
        if (value) {
          flags|=SEARCH_FLAG_SEARCH_SUBTREES;
        } else {
          flags&=(~SEARCH_FLAG_SEARCH_SUBTREES);
        }
      }
      void clear() {
        flags = SEARCH_FLAG_DEFAULT;
        max_results=-1;
      };

    };
  
  class VarItemDelegate;
  
  /*!
    \class  VarType
    \brief  The base class of the VarTypes system.
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    This is is the base-class of the VarType system.
    All other VarTypes must publicly inherit this class.
  
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */
  
  using namespace std;
  /*!
    \enum vDataFlags
    \brief bitflags for setting meta-properties of a VarType
  */
  enum VarTypeFlagEnum {
    VARTYPE_FLAG_NONE = 0x00,  /// no special properties
    VARTYPE_FLAG_READONLY= 0x01 << 0, /// this vartype is not editable in the GUI
    VARTYPE_FLAG_HIDDEN  = 0x01 << 1, /// this vartype will be hidden in the GUI
    VARTYPE_FLAG_AUTO_EXPAND = 0x01 << 2, /// whether to expand this node by default
    VARTYPE_FLAG_AUTO_EXPAND_TREE = 0x01 << 3, /// whether to expand this node and all of its sub-nodes by default
    VARTYPE_FLAG_NOSAVE = 0x01 << 4, /// don't store this subtree to XML
    VARTYPE_FLAG_NOLOAD = 0x01 << 5, /// don't load this subtree from XML
    VARTYPE_FLAG_PERSISTENT = 0x01 << 6, /// make the item editor be persistent (always open, not just when clicked on it)
    VARTYPE_FLAG_HIDE_CHILDREN = 0x01 << 7, /// hide the entire subtree (but not node itself)
    VARTYPE_FLAG_NOLOAD_ENUM_CHILDREN = 0x01 << 8, /// don't load this subtree from XML  
    VARTYPE_FLAG_NOLOAD_ATTRIBUTES = 0x01 << 9, /// only load the value, but do not load the attributes, like min/max ranges.
    VARTYPE_FLAG_SERIALIZE_EXTERNALLY = 0x01 << 10, ///serialize to external file    
    VARTYPE_FLAG_EXTERNAL_XML = 0x01 << 11, ///serialize to external file
    VARTYPE_FLAG_ENUM_COUNT = 0x01 << 12,
    VARTYPE_FLAG_NOSTORE = VARTYPE_FLAG_NOSAVE | VARTYPE_FLAG_NOLOAD
  };
  
  typedef int VarTypeFlag;
  
  
  //if using QT, trigger the hasChanged() signal
  //Note: The reason why we need a try/catch is that if shared_from_this is called when the vartype has not yet been wrapped in a shared_ptr, then a bad weak_ptr exception is thrown.
  //      This happens, for example when changed() is called from within a constructor, before the vartype was wrapped by a shared_ptr.
  //#define VARTYPE_MACRO_CHANGED try{ printf("emit: %s\n", getName().c_str()); Q_EMIT(hasChanged( //std::dynamic_pointer_cast<VarType>(((VarType*)this)->shared_from_this()))); } catch(...) { printf("FAIL\n"); } ;
  
  
  #define VARTYPE_MACRO_CHANGED try{ Q_EMIT(hasChanged( std::dynamic_pointer_cast<VarType>(((VarType*)this)->shared_from_this()))); } catch (...) { } ;

  
  #ifndef VDATA_NO_THREAD_SAFETY
    #define VARTYPE_MACRO_LOCK _mutex->lock();
    #define VARTYPE_MACRO_UNLOCK _mutex->unlock();
  #else
    #define VARTYPE_MACRO_LOCK
    #define VARTYPE_MACRO_UNLOCK
  #endif
  
  class VarType;
  typedef std::shared_ptr<VarType> VarPtr;
  
//   class QEnableSharedFromThis : public enable_shared_from_this<VarType> {
//   };
  
  //if using QT, inherit QObject as a base
  class VarType : public QObject, public virtual VarVal, public std::enable_shared_from_this<VarType>
  {
  public:

    //if using QT, enable signals by employing the q_object macro
    Q_OBJECT
  signals:
    /// This signal is triggered when any data or flag of this VarType has changed.
    /// This includes changes that were done programmatically, as well through a GUI.
    /// It also includes changes to render-flags or other meta-data.
    void hasChanged(VarPtr);
  
    /// This signal is triggered when data of this VarType has been edited by a user through an MVC viewer.
    /// Unlike /c hasChanged(), this signal is not necessarily triggered if this data was internally changed.
    /// So, if you would like to catch all events, you should use /c hasChanged() instead.
    void wasEdited(VarPtr); //this signal is only triggered when a value was edited by a user through a MVC view-instance.
  
    /// This signal is triggered if data was written from an xml node
    void XMLwasRead(VarPtr);
  
    /// This signal is triggered if data was written to an xml node
    void XMLwasWritten(VarPtr);
  
  public slots:
    /// A slot to receive signals from a model-view system that editing of this item was just completed.
    void mvcEditCompleted() {
      wasEdited(this->shared_from_this());
    }

  protected:
    virtual inline void lock() const {
      VARTYPE_MACRO_LOCK;
    }
    virtual inline void unlock() const {
      VARTYPE_MACRO_UNLOCK;
    }
    void changed() {
      VARTYPE_MACRO_CHANGED;
    }
  
    #ifndef VDATA_NO_THREAD_SAFETY
      QMutex * _mutex;
    #endif
    VarTypeFlag _flags;
    string _name;
    string _instance_description;
    string _external_xml_filename;
    
    static string fixString(const char * cst);
    static string intToString(int val);
    static string doubleToString(double val);
  
  public:
    std::string getExternalXMLFilename() const {
      std::string tmp;
      lock();
      tmp = _external_xml_filename;
      unlock();
      return tmp;
    }
    void setExternalXMLFilename(const std::string & filename) {
      lock();
      _external_xml_filename = filename;
      unlock();
    }
    QMutex * getMutex() const {
      return _mutex; 
    }
    
    /// The constructor of a VarType type. If inheriting this class, you should forward the name argument
    /// to this constructor.
    ///
    /// \param name The string label of this VarType node
    VarType(string name="");
  
    virtual ~VarType();
  

    
    
    /// Return a list of children nodes of this VarType type.
    /// This can be an empty vector if the node does not have any children.
    virtual vector<VarPtr> getChildren() const {
      vector<VarPtr> v;
      return v;
    }
  

    /// Returns all children nodes that are castable to the selected type.
    /// This can be an empty vector if the node does not have any children of that type.
    template <class t>
    vector<std::shared_ptr<t> > getCastableChildren() const {
      vector<std::shared_ptr<t> > result;
      vector<VarPtr> list = getChildren();
      ////fprintf(stderr, "NODE: %s claims to have %d children\n", getName().c_str(), list.size());
      for (int i = 0; i < list.size(); i++) {
        std::shared_ptr<t> ptr = std::dynamic_pointer_cast<t>(list[i]); 
        if (ptr.get()!=0) {
          result.push_back(ptr);
        }
      }
      return result;
    }
  
//     /// Returns all children nodes that are castable to the selected type.
//     /// This can be an empty vector if the node does not have any children of that type.
//     template <class t>
//     vector<std::shared_ptr<t> > getChildren(string blah="") const {
//       vector<std::shared_ptr<t> > result;
// //       vector<VarPtr> list = getChildren();
// //       for (int i = 0; i < list.size(); i++) {
// //         std::shared_ptr<t> ptr = std::dynamic_pointer_cast<t>(list[i]); 
// //         if (ptr.get()!=0) {
// //           result.push_back(ptr);
// //         }
// //       }
//       return result;
//     }
//     
    
    /// Checks whether this VarType can be dynamically casted to this particular type.
    template <class t>
    bool is_castable() {
      return ((std::dynamic_pointer_cast<t>(this->shared_from_this()).get())!=0);
    }
    
    /// Dynamically casts to a particular type.
    template <class t>
    std::shared_ptr<t> cast() {
      return std::dynamic_pointer_cast<t>(this->shared_from_this());
    }
    /// removes all children, by actually deleting them in memory
    /// note that this is recursive.
    /// use it to un-allocate entire hierarchies from memory
    virtual void deleteAllChildren() {
    }

    /// get the textual description of this type and what it does.
    virtual string getTypeDescription();
    
    /// get the textual description/documentation of this variable. This is typically shown inside of tool-tips.
    virtual string getInstanceDescription();
    
    /// set the textual description/documentation of this variable. This is typically shown inside of tool-tips.
    /// Note, that currently, this is not saved to/from XML.
    /// FIXME TODO: add load/store to/from XML for the description.
    virtual void setInstanceDescription(string description);
    
    /// Get the string label of the type of this VarType node. This is the same name that is registered in the VarTypesFactory instance, and the same name that will be stored as the type="" argument in the XML node during serialization.
    virtual string getTypeName();
    
    /// Set the string label of this node.
    virtual void setName(string name);
  
    /// Get the string label of this node.
    virtual string getName() const;
  
    /// Reset the values of this node to default.
    virtual void resetToDefault();
  
    /// Return the current render option bitflags
    /// \see VarTypeFlag
    virtual VarTypeFlag getFlags() const;
  
    /// Set the current render option bitflags
    /// \see VarTypeFlag
    virtual void setFlags(VarTypeFlag f);
  
    /// Add (bitwise-or) some renderflags f to the currently set renderflags.
    /// \see VarTypeFlag
    virtual void addFlags(VarTypeFlag f);
  
    /// Remove some renderflags f from the currently set renderflags.
    /// \see VarTypeFlag
    virtual void removeFlags(VarTypeFlag f);
  
    /// Check whether all the flags specified in f are currently set.
    /// \see VarTypeFlag  
    virtual bool areFlagsSet(VarTypeFlag f) const;
  
  private:
    static bool checkSearchMatch(const string & node_label, const string & search_expression, const VarTypeSearchOptions & options = VarTypeSearchOptions()) {
      return ((options.checkIgnoreCase() && (QString::fromStdString(node_label).compare(QString::fromStdString(search_expression),Qt::CaseInsensitive) == 0)) || (node_label.compare(search_expression)==0));
    };
  
  public:
    /// Finds a child based on its label and limiting search only to types that are castable to t
    /// Returns 0 if not found
    template <class t>
    std::shared_ptr<t> findCastableChild(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions() ) const {
      vector<std::shared_ptr<t> > children = getCastableChildren<t>();
      unsigned int s = children.size();
      for (unsigned int i=0;i<s;i++) {
        if (checkSearchMatch(children[i]->getName(), label)) return (children[i]);
      }
      return std::shared_ptr<t>();
    }
    
    /// Finds a child based on its label. Only the first match is returned.
    /// Returns 0 if not found
    VarPtr findChild(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      return findCastableChild<VarType>(label, options);
    }
  
    /// Finds all children based on its label and limiting search only to types that are castable to t
    /// Returns 0 if not found
    template <class t>
    vector<std::shared_ptr<t> > findCastableChildren(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      vector<std::shared_ptr<t> > children = getCastableChildren<t>();
      ////fprintf(stderr, "Found %d children of the correct type.\n", children.size());
      vector<std::shared_ptr<t> > result;
      unsigned int s = children.size();
      for (unsigned int i=0;i<s;i++) {
        ////fprintf(stderr, "Comparing (%s) to (%s)...\n", children[i]->getName().c_str(), label.c_str());
        if (checkSearchMatch(children[i]->getName(), label)) {
          ////fprintf(stderr, "Found A NODE!!!!!\n");
          result.push_back(children[i]);
        }
      }
      return result;
    }
    
  
    /// Finds all children based on its label. Only the first match is returned.
    /// Returns 0 if not found
    vector<VarPtr> findChildren(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      return findCastableChildren<VarType>(label, options);
    }
  
    //Deprecated and replaced with new and better search function:
    //     /// Recursively finds a child based on its label and limiting search only to types that are castable to t
    //     /// Returns 0 if not found
    //     template <class t>
    //     std::shared_ptr<t> findCastableChildRecursive(std::deque< string > path, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
    //       if (path.empty()) {
    //         return  std::shared_ptr<t>();
    //       } else {
    //         std::string root = path.front();
    //         path.pop_front();
    //         if (path.empty()) {
    //           return findCastableChild<t>(root, options);
    //         } else {
    //           VarPtr child = findChild(root, options);
    //           if (child.get()==0) {
    //             return std::shared_ptr<t>();
    //           } else {
    //             return child->findCastableChildRecursive<t>(path, options);
    //           }
    //         }
    //       }
    //     }
    
    /// Recursively finds a child based on its label
    /// Returns 0 if not found
    VarPtr findChildRecursive(std::deque< string > path, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      return findCastableChildRecursive<VarType>(path, options);
    }
    
    /// Recursively finds all children based on its label and limiting search only to types that are castable to t
    /// Returns number of matches and adds results into the vector v, with matching relative paths in vector p.
    template <class t>
    int findCastableChildrenRecursive(std::deque< string > orig_path, vector<std::shared_ptr<t> > & result_children, vector<std::deque< VarPtr > > & result_paths, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      
      std::deque< string > path=orig_path;
      if (path.empty()) {
        return result_children.size();
      } else {
        std::string root = path.front();
        path.pop_front();
        
        if (path.empty()) {
          vector<std::shared_ptr<t> > tchildren = findCastableChildren<t>(root, options);
          ////fprintf(stderr, "Search for castable children (%s) returned %d\n", root.c_str(), tchildren.size());
          for (int i = 0; i < tchildren.size(); i++) {
            result_children.push_back(tchildren[i]);
            result_paths.push_back(std::deque< VarPtr > ());
            if (options.getMaximumNumberOfResults()!=-1 && result_children.size() >= options.getMaximumNumberOfResults()) return result_children.size();
          }
        } else {
          vector<VarPtr > tpaths = findChildren(root, options);
          for (int i = 0; i < tpaths.size(); i++) {
            int i_last=result_paths.size();
            tpaths[i]->findCastableChildrenRecursive<t>(path, result_children, result_paths, options);
            //pre-pend root to all new results:
            for (int i=i_last; i< result_paths.size(); i++) {
              result_paths[i].push_front(tpaths[i]);
            }
            if (options.getMaximumNumberOfResults()!=-1 && result_children.size() >= options.getMaximumNumberOfResults()) return result_children.size();
          }
        }

        
        if (options.checkSearchSubtrees()) {
          vector<VarPtr> all_children = getChildren();
          int i_last=result_paths.size();
          //shift the entire search down one level into each sub-tree
          for (int i=0; i < all_children.size(); i++) {
            all_children[i]->findCastableChildrenRecursive<t>(orig_path, result_children, result_paths, options);
            if (options.getMaximumNumberOfResults()!=-1 && result_children.size() >= options.getMaximumNumberOfResults()) break;
          }
          for (int i=i_last; i< result_paths.size(); i++) {
            result_paths[i].push_front(all_children[i]);
          }
        }
      }
      return result_children.size();
    }
    
    
    ///shortcut overloaded version of findCastableChildrenRecursive
    template <class t>
    vector<std::shared_ptr<t> > findCastableChildrenRecursive(std::deque< string > path, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      vector<std::shared_ptr<t> > result;
      vector<std::deque<VarPtr> > paths;
      
      findCastableChildrenRecursive<t>(path, result, paths, options);
      
//       for (int i = 0 ; i < result.size(); i++) {
//         ////fprintf(stderr, "RESULT[%d]=%s PATHS:\n", i, result[i]->getName().c_str());
//         for (int j = 0 ; j < paths[i].size(); j++) {
//           ////fprintf(stderr, "   (%s)\n", paths[i][j]->getName().c_str());
//         }
//       }
      return result;
    }
    
    template <class t>
    std::shared_ptr<t> findCastableChildRecursive(std::deque< string > path, const VarTypeSearchOptions & options = VarTypeSearchOptions()) const {
      vector<std::shared_ptr<t> > result;
      vector<std::deque<VarPtr> > paths;
      VarTypeSearchOptions local_options = options;
      local_options.setMaximumNumberOfResults(1);
      findCastableChildrenRecursive<t>(path, result, paths, local_options);
      if (result.empty()) {
        return std::shared_ptr<t>();
      } else {
        return result[0];
      }
    }
    
    
  private:
    static std::deque<string> stringToChain(string label, char separator = '/', char escape_character = '\\') {
      int label_len=label.size();
      bool in_escape=false;
      std::string output="";
      std:deque<string> chain;
      for (int idx=0; idx < label_len; idx++) {
        char c = label[idx];
        if (c==escape_character) {
          if (in_escape) {
            in_escape=false;
            output.append(1, escape_character);
          } else {
            in_escape=true;
          }
        } else if (c==separator) {
          if (in_escape) {
            output.append(1, separator);
            in_escape=false;
          } else {
            chain.push_back(output);
            output="";
          }
        } else {
          in_escape=false;
          output.append(1,c);
        }
      }
      chain.push_back(output);
      return chain;
    }
  public:
    /// Recursively finds a child based on its label and limiting search only to types that are castable to t
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    template <class t>
    std::shared_ptr<t> findCastableChildRecursive(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildRecursive<t>(stringToChain(label, separator, escape_character), options);
    }

    
    /// Recursively finds a child based on its label
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    VarPtr findChildRecursive(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildRecursive<VarType>(label, options, separator, escape_character);
    }
 
 
    /// Recursively finds a child based on its label and limiting search only to types that are castable to t
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    template <class t>
    int findCastableChildrenRecursive(string label, vector<std::shared_ptr<t> > & result_children, vector<std::deque< VarPtr > > & result_paths, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildrenRecursive<t>(stringToChain(label, separator, escape_character), result_children, result_paths, options);
    }

    /// Overloaded shortcut version.
    /// Recursively finds a child based on its label and limiting search only to types that are castable to t
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    template <class t>
    vector<std::shared_ptr<t> > findCastableChildrenRecursive(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildrenRecursive<t>(stringToChain(label, separator, escape_character), options);
    }
    
    
    /// Recursively finds a child based on its label
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    int findChildrenRecursive(string label, vector<std::shared_ptr<VarType> > & result_children, vector<std::deque< VarPtr > > & result_paths, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildrenRecursive<VarType>(label, result_children, result_paths, options, separator, escape_character);
    }
    
    ///Overloaded shortcut version
    /// Recursively finds a child based on its label
    /// Path separator can be escaped via escape character. Escape character is printed via double escape.
    /// E.g. "Globals/Child/Yes\/No" will parse to -> "Globals"->"Child"->"Yes/No"
    /// Returns 0 if not found
    vector<std::shared_ptr<VarType> > findChildrenRecursive(string label, const VarTypeSearchOptions & options = VarTypeSearchOptions(), char separator = '/', char escape_character = '\\') const {
      return findCastableChildrenRecursive<VarType>(label, options, separator, escape_character);
    }
    
    /// TODO: implement this function. It might be useful for some purposes.
    /// VarPtr merge(VarPtr structure, VarPtr data);
  
  
  #ifndef VDATA_NO_XML
    //XML LOAD/STORE functions
  protected:
  
    //The following 6 functions are the ones to be reimplemented by subclasses to perform the correct
    //XML store/load actions
    virtual void updateAttributes(XMLNode & us) const;
    virtual void updateText(XMLNode & us) const;
    virtual void updateChildren(XMLNode & us, VarTypeImportExportOptions & options) const;
  
    virtual void readAttributes(XMLNode & us);
    virtual void readText(XMLNode & us);
    virtual void readChildren(XMLNode & us, VarTypeImportExportOptions & options);
  
    virtual void loadExternal(VarTypeImportExportOptions & options);
  
    static XMLNode findOrAppendChild(XMLNode & parent, string node_name);
  public:
    /// Clear an XMLNode's list of children.
    static void deleteAllVarChildren(XMLNode & node);
  
    /// A helper function to read a list of children from XML and convert it to a vector of VarType nodes.
    static vector<VarPtr> readChildrenHelper(XMLNode & parent , vector<VarPtr> existing_children, bool only_update_existing, bool blind_append, VarTypeImportExportOptions & options);
  
    /// Write the contents of this VarType node to an XMLNode.
    void writeXML(XMLNode & parent, VarTypeImportExportOptions & options);
  
    /// Let this VarType node load the contents of an XMLNode.
    void readXML(XMLNode & us, VarTypeImportExportOptions & options, bool is_external=false);
  #endif
  
  //-------------MODEL VIEW STUFF BELOW-------------------
  
  
  virtual QWidget * createEditor(const VarItemDelegate * delegate, QWidget *parent, const QStyleOptionViewItem &option);
  virtual void setEditorData(const VarItemDelegate * delegate, QWidget *editor) const;
  virtual void setModelData(const VarItemDelegate * delegate, QWidget *editor);
  virtual QSize sizeHint(const VarItemDelegate * delegate, const QStyleOptionViewItem & option, const QModelIndex & index) const;
  virtual void paint (const VarItemDelegate * delegate, QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
  };
  
  template <class CLASS_VARVAL_TYPE>
  class VarTypeTemplate : public VarType, virtual public CLASS_VARVAL_TYPE {
    public:
    VarTypeTemplate(string name="") : VarType(name) {
    };
  };

  template <class CLASS_VARVAL_TYPE>
  class SafeVarVal : public virtual CLASS_VARVAL_TYPE {
    protected:
    #ifndef VDATA_NO_THREAD_SAFETY
      QMutex * _mutex;
    #endif
    public:
    SafeVarVal() {
      #ifndef VDATA_NO_THREAD_SAFETY
      _mutex=new QMutex();
      #endif
    }
    virtual ~SafeVarVal()
    {
      #ifndef VDATA_NO_THREAD_SAFETY
      delete _mutex;
      #endif
    }
    protected:
    virtual inline void lock() const {
      VARTYPE_MACRO_LOCK;
    }
    virtual inline void unlock() const {
      VARTYPE_MACRO_UNLOCK;
    }
  };

};

//  Q_DECLARE_METATYPE(shared_ptr<VartYpes>)
// qRegisterMetaType<shared_ptr<SomeClass> >();

  Q_DECLARE_METATYPE ( VarTypes::VarPtr );
  //Q_DECLARE_METATYPE ( VarTypes::VarType );


#endif /*VDATATYPE_H_*/
