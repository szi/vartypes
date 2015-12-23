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
#include "VarVal.h"
#include "VarTypesInstance.h"
namespace VarTypes {
  VarVal::VarVal()
  {
  }
  
  VarVal::~VarVal()
  {
  }
  
  VarVal * VarVal::clone() const {
    return new VarVal();
  }
  
  VarVal * VarVal::deepClone() const {
    return clone();
  }
  
  void VarVal::printdebug() const {
  };
  
  string VarVal::getString() const {
    return "";
  };
  
  bool VarVal::setString(const string & val) {
    (void)val;
    return false;
  };
  
  //the following two functions are accessors for serialization
  //if this is a binary type then base64 might be a good format for i/o
  void VarVal::getSerialString(string & val) const
  {
    val=getString();
  }

  //deprecated wrapper for the above reference-based version
  string VarVal::getSerialString() const
  {
    string result;
    getSerialString(result);
    return result;
  }

  void VarVal::setSerialString(const string & val)
  {
    setString(val);
  }
  
  void VarVal::getBinarySerialString(string & val) const {
    getSerialString(val);
  };

  //deprecated wrapper for the above reference-based version
  string VarVal::getBinarySerialString() const
  {
    string result;
    getBinarySerialString(result);
    return result;
  }

  void VarVal::setBinarySerialString(const string & val) {
    setSerialString(val);
  }
  
  
  bool VarVal::serializeContentsFromStream(istream& stream, const string & extension)
  {
    std::string buf;
    stream >> buf;
    setBinarySerialString(buf);
    return true;
  }

  void VarVal::serializeContentsToStream(ostream& stream) const
  {
    std::string buf;
    getBinarySerialString(buf);
    stream << buf;
  }
  
  //the following functions are especially useful for plotting:
  bool VarVal::hasValue() const
  {
    return false;
  }
  
  bool VarVal::hasMaxValue() const
  {
    return false;
  }
  
  bool VarVal::hasMinValue() const
  {
    return false;
  }
  
  double VarVal::getMinValue() const
  {
    return 0.0;
  }
  
  double VarVal::getMaxValue() const
  {
    return 1.0;
  }
  
  double VarVal::getValue() const
  {
    return 0.0;
  }
};
