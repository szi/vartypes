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
  \file    VarProtoBufferVal.h
  \brief   C++ Interface: VarProtoBufferVal
  \author  Stefan Zickler, (C) 2008
*/

#ifndef VPROTOBUFFERVAL_H_
#define VPROTOBUFFERVAL_H_
#include "VarType.h"
//The following is not actually needed for compiling the template
//but will be needed when instanciating it.
//#include <google/protobuf/message.h>
#include "VarBase64.h"

namespace VarTypes {

  /*!
    \class  VarProtoBufferVal
    \brief  A Vartype that wraps around google protocol buffers.
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    The VarProtoBufferVal Vartype allows wrapping of Google Protocol Buffer Messages.
    It will automatically ensure that the data will be converted to/from
    base64 when storing/loading to/from XML.
    
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */
  
  template <class CLASS_VARVAL_TYPE>   
  class VarProtoBufferVal : virtual public VarVal
  {
  protected:
  
    std::tr1::shared_ptr<CLASS_VARVAL_TYPE> _val;
  
  public:

    VarProtoBufferVal(std::tr1::shared_ptr<CLASS_VARVAL_TYPE> default_val=std::tr1::shared_ptr<CLASS_VARVAL_TYPE>(new CLASS_VARVAL_TYPE())) : VarVal()
    {
      set(default_val);
      changed();
    }
  
    virtual ~VarProtoBufferVal() {
    }
  
    virtual void printdebug() const
    {
      lock();
      printf("Pointer %p\n",_val.get());
      unlock();
    }


    virtual void getSerialString(string & val) const {
      lock();
      string temp;
      if (_val.get()!=0) {
        _val->SerializeToString(&temp);
      }
      VarBase64::getTool()->encode(temp, val,1);
      unlock();
    }
  
    virtual void setSerialString(const string & val) {
      lock();
      string temp;
      if (_val.get()!=0) {
        VarBase64::getTool()->decode(val,temp);
        _val->ParseFromString(temp);
      }
      unlock();
      changed();
    }

    virtual void getBinarySerialString(string & val) const {
      lock();
      if (_val.get()!=0) _val->SerializeToString(&val);
      unlock();
    }
  
    virtual void setBinarySerialString(const string & val) {
      lock();
      if (_val.get()!=0) _val->ParseFromString(val);
      unlock();
      changed();
    }
  
    virtual VarVal * clone() const {
      VarProtoBufferVal<CLASS_VARVAL_TYPE> * tmp = new VarProtoBufferVal<CLASS_VARVAL_TYPE>();
      tmp->set(get());
      return tmp;
    }


    virtual string getString() const
    {
      CLASS_VARVAL_TYPE * tmp = get().get();
      char blah[255];
      sprintf(blah,"Pointer: %p",tmp);
      return blah;
    };

    virtual std::tr1::shared_ptr<CLASS_VARVAL_TYPE> get() const {
      std::tr1::shared_ptr<CLASS_VARVAL_TYPE> res;
      lock();
      res=_val;
      unlock();
      return res;
    };

    virtual bool set(std::tr1::shared_ptr<CLASS_VARVAL_TYPE> val) {
      lock();
      if (_val.get()!=val.get()) {
        _val=val;
        unlock();
        changed();
        return true;
      } else {
        unlock();
        return false;
      }
    };
  };

};
#endif /*VSTRING_H_*/
