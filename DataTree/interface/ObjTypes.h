//--------------------------------------------------------------------------------------------------
// $Id: ObjTypes.h,v 1.4 2009/03/12 15:55:25 bendavid Exp $
//
// ObjTypes
//
// Here we define an enum for each data class.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_OBJTYPES_H
#define MITANA_DATATREE_OBJTYPES_H
 
namespace mithep
{
  enum EObjType {
    kUnspecified = 0,
    kBaseVertex,
    kBasicCluster,
    kBeamSpot,
    kBranchName,
    kCaloTower,
    kChargedParticle,
    kCompositeParticle,
    kConversion,
    kDataBase,
    kDataObject,
    kDaughterData,
    kDecayData,
    kDecayParticle,
    kElectron,
    kEventHeader,
    kGenJet,
    kJet,
    kCaloJet,
    kPFJet,
    kLAHeader,
    kMCEventInfo,
    kMCParticle,
    kMet,
    kCaloMet,
    kPFMet,
    kMuon,
    kParticle,
    kPFCandidate,
    kPhoton,
    kRunInfo,
    kStableData,
    kStableDaughter,
    kStableParticle,
    kSuperCluster,
    kTrack,
    kTriggerMask,
    kTriggerName,
    kTriggerObject,
    kTriggerObjectBase,
    kTriggerObjectRef,
    kVertex,
    kUnknown
  };
}
#endif
