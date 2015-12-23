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
#ifndef VARTYPESFACTORY_H
#define VARTYPESFACTORY_H

  #define mRegisterVarType(VT) registerVarType<VT>(#VT);
  
  //include base-class
  #include "VarType.h"
  
  //include primitives
  #include "VarString.h"
  #include "VarInt.h"
  #include "VarBool.h"
  #include "VarDouble.h"

  #include "VarBlob.h"
  #include "VarList.h"
  #include "VarStringEnum.h"
  #include "VarSelection.h"
  #include "VarExternal.h"
  #include "VarQWidget.h"
  #include "VarTrigger.h"
  #include "VarProtoBuffer.h"
  #include "VarAny.h"

#include <typeinfo>
namespace VarTypes {  
  /**
    @author Stefan Zickler
  */

  class VarTypesFactory{
    friend class VarTypesInstance;
  public:
    VarTypesFactory();
    virtual ~VarTypesFactory();

  private:

    typedef VarPtr (*tVarTypeConstructor)();    
    template< typename T > static VarPtr fCreateVarTypePtr() {
      return VarPtr(new T());
    }
    std::map<std::string, tVarTypeConstructor> map_string_to_constructor;
    std::map<std::string, std::string> map_type_to_string;

    void registerVarTypes();
  public:
    template< typename T > bool registerVarType(const std::string & label) {
      if (isRegisteredType(label)) return false;
      map_string_to_constructor.insert(std::pair<std::string, tVarTypeConstructor>(label, &VarTypes::VarTypesFactory::fCreateVarTypePtr<T>));
      map_type_to_string.insert(std::pair<std::string, std::string>(typeid(T).name(), label));
      return true;
    }
    
    ///you should overload this function to register any vartypes you need, by calling registerVarTypes.
    virtual void registerUserVarTypes(); 

  public:
    bool                                isRegisteredType(const string & label);
    std::string                         stringFromVarType(VarPtr ptr);
    template < typename T > std::string stringFromVarType() {
      std::map<std::string, std::string>::iterator iter = map_type_to_string.find(typeid(T).name());
      if (iter==map_type_to_string.end()) {
        return "unknown";
      } else {
        return iter->second;
      }
    }
    VarPtr                              newVarType(const string & label);
    template < typename T > VarPtr      newVarType() {
      std::map<std::string, std::string>::iterator iter = map_type_to_string.find(typeid(T).name());
      if (iter==map_type_to_string.end()) {
        VarPtr ptr(0);
        return ptr;
      } else {
        return newVarType(iter->second);
      }
    }
  
  };
  
  typedef std::tr1::shared_ptr<VarTypesFactory> VarTypesFactoryPtr;
};
#endif
