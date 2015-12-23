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
  \file    VarList.h
  \brief   C++ Interface: VarList
  \author  Stefan Zickler, (C) 2008
*/

#ifndef DATAGROUP_H_
#define DATAGROUP_H_
#include "VarType.h"
#include <vector>
using namespace std;
namespace VarTypes {
  
  /*!
    \class  VarList
    \brief  This is the list type of the VarTypes system
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    VarList allows the storage of an ordered list of children nodes.
  
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */
  
  class VarList;
  typedef std::tr1::shared_ptr<VarList> VarListPtr;
  
  class VarList : public VarType
  {
    Q_OBJECT
  protected:
    vector<VarPtr> list;
  signals:
    void childAdded(VarPtr child);
    void childRemoved(VarPtr child);
  public:
    /// Creates an empty VarList
    VarList(string _name="") : VarType(_name) {
    };
    virtual ~VarList() {
      list.clear();
    };
  

    
    /// this will always return the empty string ""
    virtual string getString() const { return ""; }
  
    /// this will clear the list
    virtual void resetToDefault() {lock(); for (unsigned int i=0;i<list.size();i++) { Q_EMIT(childRemoved(list[i])); } list.clear();  unlock(); changed(); };
  
    /// prints the label and number of elements
    virtual void printdebug() const { printf("VarList named %s containing %zu element(s)\n",getName().c_str(), list.size()); }
  
    /// adds a VarType item to the end of the list.
    int addChild(VarPtr child) { lock(); list.push_back(child); Q_EMIT(childAdded(child)); unlock(); changed(); return (list.size()-1);}
    
    void copyChildrenFromVarList(VarListPtr other) {
      lock();
        list.clear();
        if (other.get()!=0) {
          list = other->getChildren();
        }
      unlock();
      changed();
    }
    
    void popLastChild() {
      lock();
      if (list.empty()==false) {
        VarPtr child = list.back();
        Q_EMIT(childRemoved(child));
        list.pop_back();
      }
      unlock();
      changed();
    }
    
    bool hasChildren() {
      bool result;
      lock();
      result = !(list.empty());
      unlock();
      return result;
    }
    
    VarPtr getLastChild() {
      VarPtr result;
      lock();
      if (list.empty()==false) {
        result = list.back();
      }
      unlock();
      return result;
    }
    
    VarPtr getChild(int idx) {
      VarPtr result;
      lock();
      if (idx >= 0 && idx < list.size() && list.empty()==false) {
        result = list[idx];
      }
      unlock();
      return result;
    }
    
    template <class t>
    std::tr1::shared_ptr<t> getCastableChild(int idx) {
      return std::tr1::dynamic_pointer_cast<t>(getChild(idx)); 
    }
    
    bool removeChild(VarPtr child) {
      lock();
      vector<VarPtr> newlist;
      unsigned int n=list.size();
      bool found=false;
      for (unsigned int i=0;i<n;i++) {
        if (list[i]!=child) {
          newlist.push_back(list[i]);
        } else {
          found=true;
        }
      }
      if (found) {
        list=newlist;
      }
      unlock();
      if (found) {
        Q_EMIT(childRemoved(child));
        changed();
      }
      return found;
    };
  
    /// returns the number of children of this list
    virtual int getChildrenCount() const
    {
      int res;
      lock();
      res = list.size();
      unlock();
      return res;
    }
  
    /// removes all children, by actually deleting them in memory
    /// note that this is recursive.
    /// use it to un-allocate entire hierarchies from memory
    virtual void deleteAllChildren() {
      lock();
//       int n = list.size();
//       for (int i = 0; i < n ; i++) {
//         if (list[i]!=0) {
//           list[i]->deleteAllChildren();
//         }
//       }
      list.clear();
      unlock();
      changed();
    }

    /// returns a vector of all children in the order that they occur in internally
    virtual vector<VarPtr> getChildren() const
    {
      lock();
      vector<VarPtr> l = list;
      unlock();
      return l;
    }
  
    /// Finds a child based on the label of 'other'
    /// If the child is not found then other is returned
    /// However, if the child *is* found then other will be *deleted* and the child will be returned!
    VarPtr findChildOrReplace(VarPtr other) {
      VarPtr data = findChild(other->getName());
      if (data.get()!=0) {
        return data;
      } else {
        addChild(other);
        return other;
      }
    }
  
    /// Finds a child based on the label of 'other'
    /// If the child is not found then other is returned
    /// However, if the child *is* found then other will be *deleted* and the child will be returned!
    template <class VCLASSPTR> 
    std::tr1::shared_ptr<VCLASSPTR> findChildOrReplace(std::tr1::shared_ptr<VCLASSPTR> other) {
      std::tr1::shared_ptr<VCLASSPTR> data = std::tr1::dynamic_pointer_cast<VCLASSPTR>(findChild(other->getName()));
      if (data.get()!=0) {
        return data;
      } else {
        addChild(other);
        return other;
      }
    }
  
  
  #ifndef VDATA_NO_XML
  protected:
    virtual void readChildren(XMLNode & us, VarTypeImportExportOptions & options)
    {
      lock();
      int before=list.size();
      list=readChildrenHelper(us, list, false, false, options);
      int after=list.size();
      if (after > before) {
        for (int i=before; i < after; i++) {
          Q_EMIT(childAdded(list[i]));
        }
      }
      unlock();
      changed();
    }
  #endif
  
  //Qt model/view gui stuff:
  public:
  virtual QWidget * createEditor(const VarItemDelegate * delegate, QWidget *parent, const QStyleOptionViewItem &option) {
    (void)delegate;
    (void)option;
    (void)parent;
    return 0;
  }

  };
};

#endif /*DATAGROUP_H_*/
