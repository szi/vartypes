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
#include "VarTypesFactory.h"


namespace VarTypes {
VarTypesFactory::VarTypesFactory()
{
  registerVarTypes();
}

VarTypesFactory::~VarTypesFactory()
{
}

bool VarTypesFactory::isRegisteredType(const std::string& label)
{
  std::map<std::string, tVarTypeConstructor>::iterator iter = map_string_to_constructor.find(label);
  if (iter==map_string_to_constructor.end()) {
    return false;
  } else {
    return true; 
  }
}

VarPtr VarTypesFactory::newVarType(const std::string & label)
{
  std::map<std::string, tVarTypeConstructor>::iterator iter = map_string_to_constructor.find(label);
  if (iter==map_string_to_constructor.end()) {
    VarPtr ptr;
    return ptr;
  } else {
    return (iter->second) ();
  }
}

void VarTypesFactory::registerUserVarTypes()
{
  ///to add you own vartypes, overload this function and register any of your types (they each need to inherit from the VarType class).
  ///To register a type, as a shortcut, you can use the mRegisterVarType(Typename) macro, or you can use the actual function:
  ///registerVarType<Typename>("Typename");
}

void VarTypesFactory::registerVarTypes()
{
  
 //Syntax for auto-generated type names.
  mRegisterVarType(VarString);
  mRegisterVarType(VarBool);
  mRegisterVarType(VarInt);
  mRegisterVarType(VarDouble);
  mRegisterVarType(VarBlob);
  mRegisterVarType(VarList);
  mRegisterVarType(VarExternal);
  mRegisterVarType(VarStringEnum);
  mRegisterVarType(VarSelection);
  mRegisterVarType(VarQWidget);
  mRegisterVarType(VarTrigger);
  //legacy type names:
//   registerVarType<VarString>("string"); 
//   registerVarType<VarBool>("bool");
//   registerVarType<VarInt>("int");
//   registerVarType<VarDouble>("double");
//   registerVarType<VarBlob>("blob");
//   registerVarType<VarList>("list");
//   registerVarType<VarExternal>("external");
//   registerVarType<VarStringEnum>("stringenum");
//   registerVarType<VarSelection>("selection");
//   registerVarType<VarQWidget>("qwidget");
//   registerVarType<VarTrigger>("trigger");
}

string VarTypesFactory::stringFromVarType(VarPtr ptr)
{
  std::string tname = (typeid(*(ptr.get())).name());  
  std::map<std::string, std::string>::iterator iter = map_type_to_string.find(tname);
  if (iter == map_type_to_string.end()) {
    return "unknown";
  } else {
    return iter->second;
  }
}




  
};
