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
#include "VarTypesInstance.h"
namespace VarTypes {
  
  VarTypesInstance* VarTypesInstance::pinstance = 0;// initialize pointer
  
  VarTypesFactoryPtr VarTypesInstance::getFactory ()
  {
    if (pinstance == 0)  // is it the first call?
    {
      pinstance = new VarTypesInstance(); // create sole instance
    }
    if (pinstance->_factory.get()==0) {
      pinstance->_factory= VarTypesFactoryPtr(new VarTypesFactory());
    }
    return pinstance->_factory; // address of sole instance
  }
  
  bool VarTypesInstance::setFactory (VarTypesFactoryPtr factory)
  {
    if (pinstance == 0)  // is it the first call?
    {
      pinstance = new VarTypesInstance(); // create sole instance
    }
    if (pinstance->_factory.get()==0) {
      factory->registerUserVarTypes();
      pinstance->_factory = factory;
      return true;
    } else {
      fprintf(stderr,"ERROR: Unable to set VarTypes factory with a VarTypesInstance::setFactory() call because getFactory() has already been called!\n");
      fprintf(stderr,"Please call VarTypesInstance::setFactory() before any other VarType usage (e.g. at the beginning of your program!)\n");
      return false;
    }
  }
  
  VarTypesInstance::VarTypesInstance()
  {
  }
};
