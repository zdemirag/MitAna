//--------------------------------------------------------------------------------------------------
// $Id: PVector.h,v 1.10 2008/07/14 09:41:59 loizides Exp $
//
// PVector
//
// Implementation of Collection interface using std:vector class for pointers pointing
// to allocated objects on heap.
//
// Persistency not tested, likely to not work, ask Constantin for help if you are in trouble.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_PVECTOR_H
#define DATATREE_PVECTOR_H
 
#include "MitAna/DataCont/interface/Vector.h"

namespace mithep 
{
  template<class ArrayElement>
  class PVector : public Vector<ArrayElement>
  {
    public:
      PVector(UInt_t rsv=0) : Vector<ArrayElement>(0) {}
      PVector(std::vector<ArrayElement> &v) : Vector<ArrayElement>(v) {}
      ~PVector() { Reset(); }

      void                             Clear(Option_t *opt="");
      void                             Reset();

    protected:

    ClassDefT(PVector, 1) // Wrapper around std::vector class
  };
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline void mithep::PVector<ArrayElement>::Clear(Option_t *opt)
{
  // Destruct container.

  Reset();
  Vector<ArrayElement>::Clear(opt);
}

//__________________________________________________________________________________________________
template<class ArrayElement>
inline void mithep::PVector<ArrayElement>::Reset()
{
  for (UInt_t i=0; i<Vector<ArrayElement>::GetEntries(); ++i) {
    delete Vector<ArrayElement>::fV[i];
  }
  Vector<ArrayElement>::fV.clear();
}
#endif