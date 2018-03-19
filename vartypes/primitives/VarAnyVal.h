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
  \file    VarAnyVal.h
  \brief   C++ Interface: VarAnyVal
  \author  Stefan Zickler, (C) 2008
*/

#ifndef VANYVAL_H_
#define VANYVAL_H_
#include "VarType.h"
//The following is not actually needed for compiling the template
//but will be needed when instanciating it.
//#include <google/protobuf/message.h>
#include "VarBase64.h"

namespace VarTypes {

  /*!
    \class  VarAnyVal
    \brief  A Vartype that wraps around anything using a shared pointer.
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    The VarAnyVal Vartype allows wrapping of any arbitrary pointer type.
    
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */
  
  template <class CLASS_VARVAL_TYPE>   
  class VarAnyVal : virtual public VarVal
  {
  protected:
    //it's got both: a plain and a shared_ptr. It prefers the shared ptr, but sometimes external non-shared pointers need to be handled.
    CLASS_VARVAL_TYPE * _ptr;
    std::shared_ptr<CLASS_VARVAL_TYPE> _shared_ptr;
    bool _auto_delete;
  public:

//     VarAnyVal(const VarAnyVal<CLASS_VARVAL_TYPE> & value) {
//       _ptr = value.get();
//       //_shared_ptr
//     }
    
    VarAnyVal(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarVal()
    {
      _auto_delete=auto_delete_pointer;
      _ptr=ptr;
      if (auto_delete_pointer) {
        _shared_ptr = std::shared_ptr<CLASS_VARVAL_TYPE>(ptr);
      }
      changed();
    }
    
    VarAnyVal(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : _shared_ptr(shared_ptr), _ptr(shared_ptr.get()), _auto_delete(false), VarVal()
    {
      changed();
    }
    
    VarAnyVal() : _ptr(0), _auto_delete(false), VarVal()
    {
      _shared_ptr = std::shared_ptr<CLASS_VARVAL_TYPE>(_ptr);
      changed();
    }
  
      
    virtual ~VarAnyVal() {
    }
  
    virtual void printdebug() const
    {
      lock();
      printf("Pointer %p\n",_ptr);
      unlock();
    }

    virtual VarVal * clone() const {
      if (_shared_ptr.get()!=0) {
        return new VarAnyVal<CLASS_VARVAL_TYPE>(_shared_ptr);
      } else {
        return new VarAnyVal<CLASS_VARVAL_TYPE>(_ptr, false);
      }
    }

    virtual string getString() const
    {
      char blah[255];
      sprintf(blah,"Pointer: %p", _ptr);
      return blah;
    };

    virtual CLASS_VARVAL_TYPE & getRef() const {
      CLASS_VARVAL_TYPE * res = get();
      return *res;
    }
    
    virtual const CLASS_VARVAL_TYPE & getConstRef() const {
      CLASS_VARVAL_TYPE * res = get();
      return *res;
    }
    
    virtual CLASS_VARVAL_TYPE * operator->() const {
      return _ptr;
    }
    
    virtual CLASS_VARVAL_TYPE * get() const {
      CLASS_VARVAL_TYPE * result;
      lock();
      result = _ptr;
      unlock();
      return result;
    }
    
    
    virtual bool set(std::shared_ptr<CLASS_VARVAL_TYPE> value) {
      lock();
      _shared_ptr=value;
      _ptr=_shared_ptr.get();
      _auto_delete=false;
      unlock();
      changed();
      return true;
    }
    
    virtual bool set(CLASS_VARVAL_TYPE * ptr, bool auto_delete=true) {
      lock();
      if (_ptr!=ptr) {
        _auto_delete=auto_delete;
        _ptr = ptr;
        if (auto_delete) {
          _shared_ptr=std::shared_ptr<CLASS_VARVAL_TYPE>(ptr);
        } else {
          _shared_ptr=std::shared_ptr<CLASS_VARVAL_TYPE>();
        }
        unlock();
        changed();
        return true;
      } else {
        _auto_delete=auto_delete;
        unlock();
        changed();
        return false;
      }
    }
    
    
//     virtual std::shared_ptr<CLASS_VARVAL_TYPE> get() const {
//       std::shared_ptr<CLASS_VARVAL_TYPE> res;
//       lock();
//       res=_ptr;
//       unlock();
//       return res;
//     };
// 
//     virtual bool set(std::shared_ptr<CLASS_VARVAL_TYPE> val) {
//       lock();
//       if (_ptr.get()!=val.get()) {
//         _ptr=val;
//         unlock();
//         changed();
//         return true;
//       } else {
//         unlock();
//         return false;
//       }
//     };
  };

};




//   template <class CLASS_VARVAL_TYPE>   
//   class VarAnyVal : virtual public VarVal
//   {
//   protected:
//   
//     std::shared_ptr<CLASS_VARVAL_TYPE> _val;
//   
//   public:
// 
//     VarAnyVal(std::shared_ptr<CLASS_VARVAL_TYPE> default_val=std::shared_ptr<CLASS_VARVAL_TYPE>(new CLASS_VARVAL_TYPE())) : VarVal()
//     {
//       set(default_val);
//       changed();
//     }
//   
//     virtual ~VarAnyVal() {
//     }
//   
//     virtual void printdebug() const
//     {
//       lock();
//       printf("Pointer %p\n",_val.get());
//       unlock();
//     }
// 
//     virtual VarVal * clone() const {
//       VarAnyVal<CLASS_VARVAL_TYPE> * tmp = new VarAnyVal<CLASS_VARVAL_TYPE>();
//       tmp->set(get());
//       return tmp;
//     }
// 
//     virtual string getString() const
//     {
//       CLASS_VARVAL_TYPE * tmp = get().get();
//       char blah[255];
//       sprintf(blah,"Pointer: %p",tmp);
//       return blah;
//     };
// 
//     virtual std::shared_ptr<CLASS_VARVAL_TYPE> get() const {
//       std::shared_ptr<CLASS_VARVAL_TYPE> res;
//       lock();
//       res=_val;
//       unlock();
//       return res;
//     };
// 
//     virtual bool set(std::shared_ptr<CLASS_VARVAL_TYPE> val) {
//       lock();
//       if (_val.get()!=val.get()) {
//         _val=val;
//         unlock();
//         changed();
//         return true;
//       } else {
//         unlock();
//         return false;
//       }
//     };
//   };







#endif /*VSTRING_H_*/
