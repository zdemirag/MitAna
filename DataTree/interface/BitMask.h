//--------------------------------------------------------------------------------------------------
// $Id: BitMask.h,v 1.4 2008/09/17 05:26:23 loizides Exp $
//
// BitMask
//
// Helper class implementing a Nx8 bit bitmask.
//
// Authors: J.Bendavid, C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_BITMASK_H
#define MITANA_DATATREE_BITMASK_H
 
#include <assert.h>
#include <Rtypes.h>

namespace mithep
{
  template<UInt_t N>
  class BitMask
  {
    public:
      BitMask() { Clear(); }
      BitMask(const char *bits) { SetBits(bits); }
      BitMask(const BitMask<N> &copy) { SetBits(copy.Bits()); }
      virtual ~BitMask() {}

      const char             *Bits()                const { return fBitMask; }
      void                    Clear()                     { memset(fBitMask,0,N); }
      void                    ClearBit(UInt_t n)          { SetBit(n,0); }
      UInt_t                  NBitsSet(UInt_t first=0, UInt_t last=N*8) const;
      void                    SetBit(UInt_t n, Bool_t b=1);
      void                    SetBits(const Char_t *bits) { memcpy(fBitMask,bits,N); }
      void                    SetBits(Long64_t bits);
      UInt_t                  Size()                const { return N*8; }
      Bool_t                  TestBit(UInt_t n)     const;
      BitMask                &operator&=(const BitMask<N> &rhs) 
        { for (UInt_t n=0; n<N; ++n) fBitMask[n]&=rhs.fBitMask[n]; return *this; }
      BitMask                &operator|=(const BitMask<N> &rhs)
        { for (UInt_t n=0; n<N; ++n) fBitMask[n]|=rhs.fBitMask[n]; return *this; }
      Bool_t                  operator!=(const BitMask<N> &other) const;
      Bool_t                  operator==(const BitMask<N> &other) const;    

    protected:
      Char_t                  fBitMask[N]; //the actual bitmask
    
    ClassDefT(BitMask, 1)
  };
}

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
inline void mithep::BitMask<N>::SetBit(UInt_t n, Bool_t b)
{
  // Set nth bit to given value.

  if(n>=N*8) return;

  UInt_t loc = n/8;
  UChar_t bit = n%8;
  if (b)
    fBitMask[loc] |= (1<<bit);
  else
    fBitMask[loc] &= (0xFF ^ (1<<bit));
}

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
UInt_t mithep::BitMask<N>::NBitsSet(UInt_t first, UInt_t last) const
{
  // Count number of bits which are set.

  UInt_t numBits = 0;
  for (UInt_t i=first; i<=last; i++)
    numBits += TestBit(i);
    
  return numBits;
}

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
inline void mithep::BitMask<N>::SetBits(Long64_t bits)
{
  // Set bits given by bits.

  assert(sizeof(Long64_t)>=N);
  SetBits(reinterpret_cast<const char*>(&bits));
}

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
inline Bool_t mithep::BitMask<N>::TestBit(UInt_t n) const
{
  // Return true if nth bit is set.

  if(n>=N*8) return 0;

  UInt_t loc = n/8;
  UChar_t bit = n%8;
  Char_t val = fBitMask[loc];
  Bool_t result = (val & (1<<bit)) != 0;
  return result;
}

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
Bool_t mithep::BitMask<N>::operator==(const mithep::BitMask<N> &other) const
{
  // Equal operator.

  for (UInt_t n=0; n<N; ++n) {
    if (fBitMask[n]!=other.fBitMask[n]) 
      return kFALSE;
  }
  return kTRUE;
}    

//--------------------------------------------------------------------------------------------------
template<UInt_t N>
Bool_t mithep::BitMask<N>::operator!=(const mithep::BitMask<N> &other) const
{
  // Unequal operator.

  for (UInt_t n=0; n<N; ++n) {
    if (fBitMask[n]==other.fBitMask[n]) 
      return kFALSE;
  }
  return kTRUE;
}    
#endif
