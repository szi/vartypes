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


namespace VarTypes {  
  /**
    @author Stefan Zickler
  */
  class VarTypesFactory{
  public:
    VarTypesFactory();
    virtual ~VarTypesFactory();
  
  protected:
    virtual VarType * newUserVarType(VarTypeId t);
    virtual VarVal  * newUserVarVal(VarTypeId t);
    virtual VarTypeId stringToUserType(const string & s);
    virtual string    userTypeToString(VarTypeId t);
  
  public:
    VarType   * newVarType(VarTypeId t);
    VarVal    * newVarVal(VarTypeId t);
    VarTypeId   stringToType(const string & s);
    string      typeToString(VarTypeId t);
  
  };
};
#endif
