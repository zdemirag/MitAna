// $Id: CompositeParticle.cc,v 1.1 2008/06/30 16:49:50 loizides Exp $

#include "MitAna/DataTree/interface/CompositeParticle.h"

ClassImp(mithep::CompositeParticle)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
Int_t CompositeParticle::Charge() const 
{
  // Return sum of charge of daughter particles.

  Int_t charge = 0;
  for (UInt_t i=0; i<GetNDaughters(); ++i)
    charge += GetDaughter(i)->Charge();
  
  return charge;
}

//--------------------------------------------------------------------------------------------------
Bool_t CompositeParticle::HasCommonDaughter(const CompositeParticle *p) const 
{
  // Return true if a common daughter exists.

  if(!p) return kFALSE;

  for (UInt_t i=0; i<p->GetNDaughters(); ++i)
    if (IsDaughter(p->GetDaughter(i)))
      return kTRUE;
	
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
Bool_t CompositeParticle::HasSameDaughters(const CompositeParticle* p) const 
{
  // Return true if daughters are the same.

  if(!p) return kFALSE;

  if (GetNDaughters()!= p->GetNDaughters())
    return kFALSE;
		
  for (UInt_t i=0; i<p->GetNDaughters(); ++i)
    if (!IsDaughter(p->GetDaughter(i))) 
        return kFALSE;
	
  return kTRUE;
}

//--------------------------------------------------------------------------------------------------
Bool_t CompositeParticle::IsDaughter(const Particle* p) const 
{
  // Return true if given particle is among daughters.

  if(!p) return kFALSE;

  if (!GetNDaughters())
    return kFALSE;

  for (UInt_t i=0; i<GetNDaughters(); ++i)
    if (GetDaughter(i)==p)
      return kTRUE;
  
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
FourVector CompositeParticle::Mom() const 
{
  // Return the vector sum of the momenta of the daughters.

  FourVector mom;
  for (UInt_t i=0; i<GetNDaughters(); ++i)
    mom += (GetDaughter(i))->Mom();
  
  return mom;
}