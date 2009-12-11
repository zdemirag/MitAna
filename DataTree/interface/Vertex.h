//--------------------------------------------------------------------------------------------------
// $Id: Vertex.h,v 1.12 2009/12/11 13:55:46 loizides Exp $
//
// Vertex
//
// Vertex class derived from BaseVertex holding additional fit information.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_VERTEX_H
#define MITANA_DATATREE_VERTEX_H
 
#include <TMath.h>
#include "MitAna/DataTree/interface/BaseVertex.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataTree/interface/Track.h"

namespace mithep 
{
  class Vertex : public BaseVertex
  {
    public:
      Vertex() : fChi2(0), fIsValid(kFALSE), fNdof(0), fNTracks(0) {}
      Vertex(Double_t x, Double_t y, Double_t z) : 
        BaseVertex(x,y,z), fChi2(0), fIsValid(kFALSE), fNdof(0), fNTracks(0) {}
      Vertex(Double_t x, Double_t y, Double_t z, Double_t xErr, Double_t yErr, Double_t zErr) :
        BaseVertex(x,y,z,xErr,yErr,zErr), fChi2(0), fIsValid(kFALSE), fNdof(0), fNTracks(0) {}
      Vertex(const ThreeVector &pos) : 
        BaseVertex(pos), fChi2(0), fNdof(0), fNTracks(0) {}
      
      void                AddTrack(const Track *t) { fTracks.Add(t);           }
      Double_t            Chi2()      const { return fChi2;                    } 
      Int_t               Compare(const TObject *o) const;
      Bool_t              HasTrack(const Track *t)  const { return fTracks.HasObject(t); }
      Bool_t              IsSortable() const { return kTRUE;                    }
      Bool_t              IsValid()   const { return fIsValid;                 }
      UShort_t            Ndof()      const { return fNdof;                    }
      UInt_t              NTracksFit() const { return fNTracks;                }
      UInt_t              NTracks()   const { return fTracks.Entries();        }
      EObjType            ObjType()   const { return kVertex;                  }      
      Double_t            Prob()      const { return TMath::Prob(fChi2,fNdof); }
      void                SetChi2(Double_t chi2)     { fChi2 = chi2;           }
      void                SetIsValid(Bool_t b)       { fIsValid = b;           }
      void                SetNdof(UShort_t nDof)     { fNdof = nDof;           } 
      void                SetNTracksFit(UInt_t n)    { fNTracks = n;           }
      const Track        *Trk(UInt_t i) const        { return fTracks.At(i);   }
            
    protected:
      Double32_t          fChi2;     //[0,0,12]chi squared of conversion vertex fit
      Bool_t              fIsValid;  //is vertex valid
      UShort_t            fNdof;     //number of degrees of freedom of conversion vertex fit
      UShort_t            fNTracks;  //number of tracks used for the fit
      RefArray<Track>     fTracks;   //tracks associated with the PV
	
    ClassDef(Vertex, 2) // Vertex class
  };
}

//--------------------------------------------------------------------------------------------------
inline Int_t mithep::Vertex::Compare(const TObject *o) const
{
  // Default compare function for sorting according to transverse momentum. 
  // Returns -1 if this object is smaller than given object, 0 if objects are 
  // equal and 1 if this is larger than given object.

  const mithep::Vertex *v = dynamic_cast<const mithep::Vertex *>(o);
  if (!v)
    return 1;

  Int_t myn = NTracks();
  Int_t n   = v->NTracks();
  if (myn>n)
    return -1;
  else if (n>myn)
    return +1;

  Double_t myd = Chi2();
  Double_t d   = v->Chi2();
  if (myd<d)
    return -1;
  else if (d<myd)
    return +1;
  
  return 0;
}
#endif
