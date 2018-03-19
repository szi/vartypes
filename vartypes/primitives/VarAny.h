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
  \file    VarAny.h
  \brief   C++ Interface: VarAny
  \author  Stefan Zickler, (C) 2008
*/

#ifndef VANY_H_
#define VANY_H_
#include "VarType.h"
#include "VarAnyVal.h"
#include <QSpinBox>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
//#include "serializers.h"
namespace VarTypes {
  /*!
    \class  VarAny
    \brief  A Vartype for storing anything wrapped in a shared pointer
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */ 
  
//   template <class CLASS_VARVAL_TYPE>   
//   class VarAny : public VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> > 
//   {
//   public:
//     virtual inline void changed() {
//       VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >::changed();
//     }
//     VarAny(string name="") : VarAnyVal<CLASS_VARVAL_TYPE>(new CLASS_VARVAL_TYPE()), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//       fprintf(stderr, "VARANY Constructor: name: %s\n", name.c_str());
//       fprintf(stderr, "VARANY Constructor: get function: %p\n", VarAnyVal<CLASS_VARVAL_TYPE>::_ptr);
//     }
//     VarAny(string name, CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAnyVal<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//     }
//     
//     VarAny(string name, std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAnyVal<CLASS_VARVAL_TYPE>(shared_ptr), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//     }
//     
//     //nameless shortcuts:
//     VarAny(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAnyVal<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
//     {
//     }
//     
//     VarAny(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAnyVal<CLASS_VARVAL_TYPE>(shared_ptr), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
//     {
//     }
//     
//     virtual ~VarAny() {}
//   };



  template <class CLASS_VARVAL_TYPE>   
  class VarAny : public VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> > 
  {
  public:
    virtual inline void changed() {
      VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >::changed();
    }
    VarAny(string name="") : VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
    {
      VarAnyVal<CLASS_VARVAL_TYPE>::set(new CLASS_VARVAL_TYPE()); 
    }
    VarAny(string name, CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
    {
      VarAnyVal<CLASS_VARVAL_TYPE>::set(ptr, auto_delete_pointer); 
    }
    
    VarAny(string name, std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
    {
      VarAnyVal<CLASS_VARVAL_TYPE>::set(shared_ptr); 
    }
    
    //nameless shortcuts:
    VarAny(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
    {
      VarAnyVal<CLASS_VARVAL_TYPE>::set(ptr, auto_delete_pointer); 
    }
    
    VarAny(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
    {
      VarAnyVal<CLASS_VARVAL_TYPE>::set(shared_ptr); 
    }
    
    virtual ~VarAny() {}
  };





  /*
  template <class CLASS_VARVAL_TYPE>   
  class VarAny : public VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> > 
  {
  public:
    virtual inline void changed() {
      VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >::changed();
    }
    VarAny(string name="") : VarAnyVal<CLASS_VARVAL_TYPE>(), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
    {
    }
    virtual ~VarAny() {}
  };*/
  
  template <class CLASS_VARVAL_TYPE>   
  class VarAnyProtoObject : public VarAny<CLASS_VARVAL_TYPE>
  {
  public:
    
    VarAnyProtoObject(string name="") : VarAny<CLASS_VARVAL_TYPE>(name)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyProtoObject(string name, CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAny<CLASS_VARVAL_TYPE>(name, ptr, auto_delete_pointer)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyProtoObject(string name, std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAny<CLASS_VARVAL_TYPE>(name, shared_ptr)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    //nameless shortcuts:
    VarAnyProtoObject(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAny<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyProtoObject(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAny<CLASS_VARVAL_TYPE>(shared_ptr)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    virtual void getSerialString(string & val) const {
      std::string tmp;
      VarAny<CLASS_VARVAL_TYPE>::get()->SerializeToString(&tmp);
      VarBase64::getTool()->encode(tmp, val,0);
    }
  
    virtual void setSerialString(const string & val) {
      std::string tmp;
      VarBase64::getTool()->decode(val,tmp);
      VarAny<CLASS_VARVAL_TYPE>::get()->ParseFromString(tmp);
      VarAny<CLASS_VARVAL_TYPE>::changed();
    }
    
  
    virtual bool serializeContentsFromStream(std::istream & stream, const std::string & extension=".bin") {
      bool success = VarAny<CLASS_VARVAL_TYPE>::get()->ParseFromIstream(&stream);
      VarAny<CLASS_VARVAL_TYPE>::changed();
      return success;
    }
  
    virtual void serializeContentsToStream(std::ostream & stream) const {
      VarAny<CLASS_VARVAL_TYPE>::get()->SerializeToOstream(&stream);
    }
    
    virtual ~VarAnyProtoObject() {}
  };
  
  template <class CLASS_VARVAL_TYPE>   
  class VarAnyBoostSerializableObject : public VarAny<CLASS_VARVAL_TYPE>
  {
  public:
    
    
    VarAnyBoostSerializableObject(string name="") : VarAny<CLASS_VARVAL_TYPE>(name)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyBoostSerializableObject(string name, CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAny<CLASS_VARVAL_TYPE>(name, ptr, auto_delete_pointer)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyBoostSerializableObject(string name, std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAny<CLASS_VARVAL_TYPE>(name, shared_ptr)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    //nameless shortcuts:
    VarAnyBoostSerializableObject(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAny<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
    VarAnyBoostSerializableObject(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAny<CLASS_VARVAL_TYPE>(shared_ptr)
    {
      VarType::addFlags(VARTYPE_FLAG_SERIALIZE_EXTERNALLY);
    }
    
//     VarAnyBoostSerializableObject(string name="") : VarAnyVal<CLASS_VARVAL_TYPE>(new CLASS_VARVAL_TYPE()), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//       fprintf(stderr, "VARANY Constructor: name: %s\n", name.c_str());
//       fprintf(stderr, "VARANY Constructor: get function: %p\n", VarAnyVal<CLASS_VARVAL_TYPE>::_ptr);
//     }
//     VarAnyBoostSerializableObject(string name, CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAnyVal<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//     }
//     
//     VarAnyBoostSerializableObject(string name, std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAnyVal<CLASS_VARVAL_TYPE>(shared_ptr), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >(name)
//     {
//     }
//     
//     //nameless shortcuts:
//     VarAnyBoostSerializableObject(CLASS_VARVAL_TYPE * ptr, bool auto_delete_pointer=true) : VarAnyVal<CLASS_VARVAL_TYPE>(ptr, auto_delete_pointer), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
//     {
//     }
//     
//     VarAnyBoostSerializableObject(std::shared_ptr<CLASS_VARVAL_TYPE> shared_ptr) : VarAnyVal<CLASS_VARVAL_TYPE>(shared_ptr), VarTypeTemplate<VarAnyVal<CLASS_VARVAL_TYPE> >("")
//     {
//     } 
    
    virtual void getSerialString(string & val) const {
      std::stringstream ss;
      boost::archive::binary_oarchive oa(ss);
      oa << *(VarAny<CLASS_VARVAL_TYPE>::get());
      VarBase64::getTool()->encode(ss.str(), val,0);
    }
  
    virtual void setSerialString(const string & val) {
      std::string tmp;
      VarBase64::getTool()->decode(val,tmp);
      stringstream ss(tmp);
      boost::archive::binary_iarchive ia(ss);
      ia >> *(VarAny<CLASS_VARVAL_TYPE>::get());
      VarAny<CLASS_VARVAL_TYPE>::changed();
    }

    virtual bool serializeContentsFromStream(std::istream & stream, const std::string & extension=".bin") {
      boost::archive::binary_iarchive ia(stream);
      ia >> *(VarAny<CLASS_VARVAL_TYPE>::get());
      VarAny<CLASS_VARVAL_TYPE>::changed();
      return true;
    }
  
    virtual void serializeContentsToStream(std::ostream & stream) const {
      boost::archive::binary_oarchive oa(stream);
      oa << *(VarAny<CLASS_VARVAL_TYPE>::get());
    }
    
    virtual ~VarAnyBoostSerializableObject() {
      
    }
  };
  
  
};


#endif /*VINTEGER_H_*/
