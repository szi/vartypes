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
  \file    VarProtoBuffer.h
  \brief   C++ Interface: VarProtoBuffer
  \author  Stefan Zickler, (C) 2008
*/

#ifndef VPROTOBUFFER_H_
#define VPROTOBUFFER_H_
#include "VarType.h"
#include "VarProtoBufferVal.h"
#include <QSpinBox>
namespace VarTypes {
  /*!
    \class  VarProtoBuffer
    \brief  A Vartype for storing integers
    \author Stefan Zickler, (C) 2008
    \see    VarTypes.h
  
    If you don't know what VarTypes are, please see \c VarTypes.h 
  */ 
  
  template <class CLASS_VARVAL_TYPE>   
  class VarProtoBuffer : public VarTypeTemplate<VarProtoBufferVal<CLASS_VARVAL_TYPE> > 
  {

  public:
  
    virtual inline void changed() {
      VarTypeTemplate<VarProtoBufferVal<CLASS_VARVAL_TYPE> >::changed();
    }
    VarProtoBuffer(string name="") : VarProtoBufferVal<CLASS_VARVAL_TYPE>(), VarTypeTemplate<VarProtoBufferVal<CLASS_VARVAL_TYPE> >(name)
    {
    }
  
    virtual ~VarProtoBuffer() {}

  };
};


#endif /*VINTEGER_H_*/
