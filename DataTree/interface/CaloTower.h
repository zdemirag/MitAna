//--------------------------------------------------------------------------------------------------
// $Id: CaloTower.h,v 1.12 2009/05/18 06:29:57 loizides Exp $
//
// CaloTower
//
// This class holds calo tower information. Note that the energy of the tower does not 
// necessarily have to be greater 0, and as such E(), Et() (or Pt()) can return negative
// values.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_CALOTOWER_H
#define MITANA_DATATREE_CALOTOWER_H
 
#include <TMath.h>
#include "MitCommon/DataFormats/interface/Vect3C.h"
#include "MitAna/DataCont/interface/CacheFlag.h"
#include "MitAna/DataTree/interface/DataObject.h"

namespace mithep 
{
  class CaloTower : public DataObject
  {
    public:
      CaloTower() : fEmEnergy(0), fHadEnergy(0), fOuterEnergy(0) {}

      Double_t             E()           const { return (fEmEnergy + fHadEnergy);                }
      Double_t             EmEt()        const { return fEmEnergy*TMath::Sin(Theta());           }
      Double_t             Eta()         const { return fPosition.Eta();                         }
      Double_t             Et()          const { return E()*TMath::Sin(Theta());                 }
      Double_t             EtWithHO()    const { return EWithHO()*TMath::Sin(Theta());           }
      Double_t             EWithHO()     const { return (fEmEnergy + fHadEnergy + fOuterEnergy); }
      Double_t             EmEnergy()    const { return fEmEnergy;                               }
      Double_t             HadEnergy()   const { return fHadEnergy;                              } 
      Double_t             HadEt()       const { return fHadEnergy*TMath::Sin(Theta());          }
      const FourVectorM   &Mom()         const;
      Double_t             Phi()         const { return fPosition.Phi();                         }
      Double_t             Pt()          const { return Et(); /*careful can become negative*/    }
      EObjType             ObjType()     const { return kCaloTower;                              }
      Double_t             OuterEnergy() const { return fOuterEnergy;                            }
      Double_t             OuterEt()     const { return fOuterEnergy*TMath::Sin(Theta());        }
      const ThreeVectorC  &Position()    const;
      Double_t             Theta()       const { return Position().Theta();                      }
      void	           SetEmEnergy(Double_t EmEnergy)       
                             { fEmEnergy    = EmEnergy;  ClearMom(); }
      void	           SetHadEnergy(Double_t HadEnergy)     
                             { fHadEnergy   = HadEnergy; ClearMom(); }
      void	           SetOuterEnergy(Double_t OuterEnergy) 
                             { fOuterEnergy = OuterEnergy; ClearMom(); }
      void                 SetPosition(Double_t x, Double_t y, Double_t z)
                             { fPosition.SetXYZ(x,y,z); ClearMom(); ClearPos(); }

    protected:
      void                 ClearMom()    const { fCacheMomFlag.ClearCache(); }
      void                 ClearPos()    const { fCachePosFlag.ClearCache(); }
      void                 GetMom()      const;
      void                 GetPos()      const;
    
      Vect3C               fPosition;     //position of tower
      Double32_t           fEmEnergy;     //[0,0,14]tower energy in Ecal
      Double32_t           fHadEnergy;    //[0,0,14]tower energy in Hcal
      Double32_t           fOuterEnergy;  //[0,0,14]tower energy in outer Hcal
      mutable CacheFlag    fCacheMomFlag; //||cache validity flag for momentum
      mutable FourVectorM  fCachedMom;    //!cached momentum vector
      mutable CacheFlag    fCachePosFlag; //||cache validity flag for position
      mutable ThreeVectorC fCachedPos;    //!cached position vector

    ClassDef(CaloTower, 1) // Calo tower class
  };
}

//--------------------------------------------------------------------------------------------------
inline void mithep::CaloTower::GetMom() const
{
  // Compute four momentum.

  if (E() > 0)
    fCachedMom.SetCoordinates(Et(),Eta(),Phi(),0.0);
  else
    fCachedMom = mithep::FourVectorM();
}

//--------------------------------------------------------------------------------------------------
inline void mithep::CaloTower::GetPos() const
{
  // Compute position.

  fCachedPos.SetCoordinates(fPosition.Rho(), fPosition.Eta(), fPosition.Phi());
}

//--------------------------------------------------------------------------------------------------
inline const mithep::FourVectorM &mithep::CaloTower::Mom() const
{
  // Return cached momentum value.

  if (!fCacheMomFlag.IsValid()) {
    GetMom();
    fCacheMomFlag.SetValid();
  }
  return fCachedMom;
}

//--------------------------------------------------------------------------------------------------
inline const mithep::ThreeVectorC &mithep::CaloTower::Position() const
{
  // Return cached momentum value.

  if (!fCachePosFlag.IsValid()) {
    GetPos();
    fCachePosFlag.SetValid();
  }
  return fCachedPos;
}
#endif
