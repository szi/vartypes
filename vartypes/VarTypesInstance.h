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
#ifndef VARTYPESINSTANCE_H
#define VARTYPESINSTANCE_H
#include "VarTypesFactory.h"
/**
	@author Stefan Zickler
*/
namespace VarTypes {
  /*!
    \class  VarTypesInstance
    \brief  A singleton wrapper to VarTypesFactory
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    This is a singleton wrapper to the VarTypesFactory
    This allows global registration of a custom VarTypesFactory subclass,
    by calling VarTypesInstance::setFactory(customVarTypesFactorySubClass)
    before using any other vartypes functions. This in turn provides the
    simple ability to add additional types to the vartypes system without
    touching the core files.
    If setFactory is not called before getFactory() is used, then
    the VarTypes' default factory class "VarTypesFactory" will be
    auto-instanciated instead.
  
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */
  //singleton instanciation of XML base64 encoder
  class VarTypesInstance
    {
    public:
        static VarTypesFactory* getFactory();
        static bool setFactory(VarTypesFactory * factory);
    protected:
        VarTypesInstance();
        VarTypesInstance(const VarTypesInstance&);
        VarTypesInstance& operator= (const VarTypesInstance&);
    private:
        static VarTypesInstance* pinstance;
        VarTypesFactory * _factory;
  };
};  
#endif
