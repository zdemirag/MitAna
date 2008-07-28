//--------------------------------------------------------------------------------------------------
// $Id: Collections.h,v 1.15 2008/07/25 16:04:44 bendavid Exp $
//
// Collections
//
// This header file defines the standard types for the collections we want to use.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_COLLECTIONS_H
#define DATATREE_COLLECTIONS_H
 
#include "MitAna/DataTree/interface/Collection.h"
#include "MitAna/DataTree/interface/Array.h"
#include "MitAna/DataTree/interface/ObjArray.h"
#include "MitAna/DataTree/interface/DataObject.h"
#include "MitAna/DataTree/interface/Track.h"
#include "MitAna/DataTree/interface/Particle.h"
#include "MitAna/DataTree/interface/ChargedParticle.h"
#include "MitAna/DataTree/interface/Muon.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/Jet.h"
#include "MitAna/DataTree/interface/Met.h"
#include "MitAna/DataTree/interface/CompositeParticle.h"
#include "MitAna/DataTree/interface/MCParticle.h"
#include "MitAna/DataTree/interface/DecayParticle.h"
#include "MitAna/DataTree/interface/Photon.h"
#include "MitAna/DataTree/interface/Conversion.h"
#include "MitAna/DataTree/interface/BasePart.h"
#include "MitAna/DataTree/interface/DecayPart.h"
#include "MitAna/DataTree/interface/StablePart.h"
#include "MitAna/DataTree/interface/StableParticle.h"

namespace mithep 
{
  typedef Collection<DataObject>        DataObjectCol;
  typedef Collection<Track>             TrackCol;
  typedef Collection<Particle>          ParticleCol;
  typedef Collection<ChargedParticle>   ChargedParticleCol;
  typedef Collection<Muon>              MuonCol;
  typedef Collection<Electron>          ElectronCol;
  typedef Collection<Jet>               JetCol;
  typedef Collection<Met>               MetCol;
  typedef Collection<CompositeParticle> CompositeParticleCol;
  typedef Collection<MCParticle>        MCParticleCol;
  typedef Collection<DecayParticle>     DecayParticleCol;
  typedef Collection<Photon>            PhotonCol;
  typedef Collection<Conversion>        ConversionCol;
  typedef Collection<Conversion>        StableParticleCol;

  typedef Collection<BasePart>          BasePartCol;

  typedef Array<DataObject>             DataObjectArr;
  typedef Array<Track>                  TrackArr;
  typedef Array<Conversion>             ConversionArr;
  typedef Array<Muon>                   MuonArr;
  typedef Array<Electron>               ElectronArr;
  typedef Array<Jet>                    JetArr;
  typedef Array<Met>                    MetArr;
  typedef Array<MCParticle>             MCParticleArr;
  typedef Array<DecayParticle>          DecayParticleArr;
  typedef Array<Photon>                 PhotonArr;
  typedef ObjArray<BasePart>            BasePartObjArr;
  typedef ObjArray<DecayPart>           DecayPartObjArr;
  typedef ObjArray<StablePart>          StablePartObjArr;
  typedef Array<Conversion>             StableParticleArr;
  typedef Array<Conversion>             ConversionArr;
}
#endif
