//This class provides an interface to allow exporting vartypes as runtime-loadable "plugins" / shared libraries (e.g. .so files).
#ifndef VARTYPES_LOADER_INTERFACE_H
#define VARTYPES_LOADER_INTERFACE_H
#include "VarTypesFactory.h"

class VarTypesLoaderInterface {
public:
  virtual void registerTypes(VarTypes::VarTypesFactoryPtr factory) = 0;
};

Q_DECLARE_INTERFACE(VarTypesLoaderInterface, "VarTypes.LoaderInterface")

#endif
