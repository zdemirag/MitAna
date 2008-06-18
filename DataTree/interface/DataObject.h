//--------------------------------------------------------------------------------------------------
// $Id: DataObject.h,v 1.3 2008/06/17 14:47:16 loizides Exp $
//
// DataObject
//
// This is the common base class for all objects in the tree. 
//
// Authors: C.Loizides
//
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_DATAOBJECT_H
#define DATATREE_DATAOBJECT_H
 
#include <TObject.h>
#include "MitAna/DataTree/interface/Types.h"

namespace mithep 
{
  class DataObject : public TObject
  {
    public:
      DataObject() {}
      ~DataObject() {}
  
      ClassDef(DataObject, 1) // Basic data object class
  };
}
#endif
