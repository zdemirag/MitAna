//--------------------------------------------------------------------------------------------------
// $Id: Collections.h,v 1.10 2008/07/01 14:09:27 loizides Exp $
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
#include "MitAna/DataTree/interface/DataObject.h"
#include "MitAna/DataTree/interface/Track.h"
#include "MitAna/DataTree/interface/Particle.h"
#include "MitAna/DataTree/interface/Lepton.h"
#include "MitAna/DataTree/interface/Muon.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/CompositeParticle.h"
#include "MitAna/DataTree/interface/GenParticle.h"
#include "MitAna/DataTree/interface/SimParticle.h"
#include "MitAna/DataTree/interface/DecayParticle.h"
#include "MitAna/DataTree/interface/Photon.h"

namespace mithep 
{
  typedef Collection<DataObject>        DataObjectCol;
  typedef Collection<Track>             TrackCol;
  typedef Collection<Particle>          ParticleCol;
  typedef Collection<Lepton>            LeptonCol;
  typedef Collection<Muon>              MuonCol;
  typedef Collection<Electron>          ElectronCol;
  typedef Collection<CompositeParticle> CompositeParticleCol;
  typedef Collection<GenParticle>       GenParticleCol;
  typedef Collection<SimParticle>       SimParticleCol;
  typedef Collection<DecayParticle>     DecayParticleCol;
  typedef Collection<Photon>            PhotonCol;
  typedef Collection<Conversion>        ConversionCol;

  typedef Array<DataObject>             DataObjectArr;
  typedef Array<Track>                  TrackArr;
  typedef Array<Particle>               ParticleArr;
  typedef Array<Lepton>                 LeptonArr;
  typedef Array<Muon>                   MuonArr;
  typedef Array<Electron>               ElectronArr;
  typedef Array<GenParticle>            GenParticleArr;
  typedef Array<SimParticle>            SimParticleArr;
  typedef Array<DecayParticle>          DecayParticleArr;
  typedef Array<Photon>                 PhotonArr;
  typedef Array<Conversion>             ConversionArr;
}
#endif
