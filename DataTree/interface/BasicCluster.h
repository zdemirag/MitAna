//--------------------------------------------------------------------------------------------------
// $Id: BasicCluster.h,v 1.21 2012/04/20 16:06:55 bendavid Exp $
//
// BasicCluster
//
// This class holds information of basic reconstructed clusters.
//
// Authors: C.Paus, J.Bendavid, S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_BASICCLUSTER_H
#define MITANA_DATATREE_BASICCLUSTER_H
 
#include <TMath.h>
#include "MitCommon/DataFormats/interface/Vect3C.h"
#include "MitAna/DataTree/interface/DataObject.h"

namespace mithep 
{
  class BasicCluster : public DataObject
  {
  public:
    BasicCluster() :
      fEnergy(0),
      //fPoint(0),
      fNHits(0),
      fE1x3(0),fE3x1(0),fE1x5(0),fE2x2(0),fE3x2(0),fE3x3(0),fE4x4(0),fE5x5(0),
      fSwissCross(0),
      fE2x5Right(0),fE2x5Left(0),fE2x5Top(0),fE2x5Bottom(0),fE2x5Max(0),
      fELeft(0),fERight(0),fETop(0),fEBottom(0),
      fEMax(0),
      fE2nd(0),
      fEtaLat(0),fPhiLat(0),fLat(0),
      fCovEtaEta(0),fCovEtaPhi(0),fCovPhiPhi(0),fCoviEtaiEta(0),fCoviEtaiPhi(0),fCoviPhiiPhi(0),
      fZernike20(0),fZernike42(0),
      fEtaCry(-99.), fPhiCry(-99.), fXCry(-99.), fYCry(-99.),
      fThetaAxis(0.), fPhiAxis(0.),
      fIEta(-999), fIPhi(-999), fIX(-999), fIY(-999),
      fSeedIEta(-999), fSeedIPhi(-999), fSeedIX(-999), fSeedIY(-999),
      fInsideMustache(kFALSE), fTime(0.), fSeedTime(0.), fMatchedEnergy(0.)
    {}
    
    BasicCluster(Double_t e, const ThreeVector &p) : 
      fEnergy(e),
      fPoint(p),
      fNHits(0),
      fE1x3(0),fE3x1(0),fE1x5(0),fE2x2(0),fE3x2(0),fE3x3(0),fE4x4(0),fE5x5(0),
      fSwissCross(0),
      fE2x5Right(0),fE2x5Left(0),fE2x5Top(0),fE2x5Bottom(0),fE2x5Max(0),
      fELeft(0),fERight(0),fETop(0),fEBottom(0),
      fEMax(0),
      fE2nd(0),
      fEtaLat(0),fPhiLat(0),fLat(0),
      fCovEtaEta(0),fCovEtaPhi(0),fCovPhiPhi(0),fCoviEtaiEta(0),fCoviEtaiPhi(0),fCoviPhiiPhi(0),
      fZernike20(0),fZernike42(0),
      fEtaCry(-99.), fPhiCry(-99.), fXCry(-99.), fYCry(-99.),
      fThetaAxis(0.), fPhiAxis(0.),
      fIEta(-999), fIPhi(-999), fIX(-999), fIY(-999),
      fSeedIEta(-999), fSeedIPhi(-999), fSeedIX(-999), fSeedIY(-999),
      fInsideMustache(kFALSE), fTime(0.), fSeedTime(0.), fMatchedEnergy(0.)
    {}
    
    Double_t     Energy()                  const { return  fEnergy;       }       
    Double_t     Et()                      const;
    Double_t     Eta()                     const { return  fPoint.Eta();  }
    EObjType     ObjType()                 const { return  kBasicCluster; }       
    Double_t     Phi()                     const { return  fPoint.Phi();  }
    ThreeVectorC Pos()                     const { return  fPoint.V();    }
    void         Print(Option_t *opt="")   const;
    Double_t     Rho()                     const { return  fPoint.Rho();  }
    Int_t        NHits()                   const { return  fNHits;        }
    Double_t     E1x3()                    const { return  fE1x3;         }
    Double_t     E3x1()                    const { return  fE3x1;         }
    Double_t     E1x5()                    const { return  fE1x5;         }
    Double_t     E2x2()                    const { return  fE2x2;         }
    Double_t     E3x2()                    const { return  fE3x2;         }
    Double_t     E3x3()                    const { return  fE3x3;         }
    Double_t     E4x4()                    const { return  fE4x4;         }
    Double_t     E5x5()                    const { return  fE5x5;         }
    Double_t     SwissCross()              const { return  fSwissCross;   }
    Double_t     E2x5Right()               const { return  fE2x5Right;    }
    Double_t     E2x5Left()                const { return  fE2x5Left;     }
    Double_t     E2x5Top()                 const { return  fE2x5Top;      }
    Double_t     E2x5Bottom()              const { return  fE2x5Bottom;   }
    Double_t     E2x5Max()                 const { return  fE2x5Max;      }
    Double_t     ELeft()                   const { return  fELeft;        }
    Double_t     ERight()                  const { return  fERight;       }
    Double_t     ETop()                    const { return  fETop;         }
    Double_t     EBottom()                 const { return  fEBottom;      }
    Double_t     EMax()                    const { return  fEMax;         }
    Double_t     E2nd()                    const { return  fE2nd;         }
    Double_t     EtaLat()                  const { return  fEtaLat;       }
    Double_t     PhiLat()                  const { return  fPhiLat;       }
    Double_t     Lat()                     const { return  fLat;          }
    Double_t     CovEtaEta()               const { return  fCovEtaEta;    }
    Double_t     CovEtaPhi()               const { return  fCovEtaPhi;    }
    Double_t     CovPhiPhi()               const { return  fCovPhiPhi;    }
    Double_t     CoviEtaiEta()             const { return  fCoviEtaiEta;  }
    Double_t     CoviEtaiPhi()             const { return  fCoviEtaiPhi;  }
    Double_t     CoviPhiiPhi()             const { return  fCoviPhiiPhi;  }
    Double_t     Zernike20()               const { return  fZernike20;    }
    Double_t     Zernike42()               const { return  fZernike42;    }
    Double_t     EtaCry()                  const { return  fEtaCry;       }
    Double_t     PhiCry()                  const { return  fPhiCry;       }
    Double_t     XCry()                    const { return  fXCry;         }
    Double_t     YCry()                    const { return  fYCry;         }
    Double_t     ThetaAxis()               const { return  fThetaAxis;    }
    Double_t     PhiAxis()                 const { return  fPhiAxis;      }
    Short_t      IEta()                    const { return  fIEta;         }
    Short_t      IPhi()                    const { return  fIPhi;         }
    Short_t      IX()                      const { return  fIX;           }
    Short_t      IY()                      const { return  fIY;           }
    Short_t      SeedIEta()                const { return  fSeedIEta;     }
    Short_t      SeedIPhi()                const { return  fSeedIPhi;     }
    Short_t      SeedIX()                  const { return  fSeedIX;       }
    Short_t      SeedIY()                  const { return  fSeedIY;       }
    Bool_t       InsideMustache()          const { return  fInsideMustache; }
    Double_t     Time()                    const { return  fTime;           }
    Double_t     SeedTime()                const { return  fSeedTime;       }
    Double_t     MatchedEnergy()           const { return  fMatchedEnergy;  }
    
    void	 SetEnergy(Double_t energy)                 { fEnergy = energy;     }    
    void	 SetXYZ(Double_t x, Double_t y, Double_t z) { fPoint.SetXYZ(x,y,z); } 
    void	 SetNHits(Int_t x)                          { fNHits = x;           }
    void	 SetE1x3(Double_t x)                        { fE1x3 = x;            }
    void	 SetE3x1(Double_t x)                        { fE3x1 = x;            }
    void	 SetE1x5(Double_t x)                        { fE1x5 = x;            }
    void	 SetE2x2(Double_t x)                        { fE2x2 = x;            }
    void	 SetE3x2(Double_t x)                        { fE3x2 = x;            }
    void	 SetE3x3(Double_t x)                        { fE3x3 = x;            }
    void	 SetE4x4(Double_t x)                        { fE4x4 = x;            }
    void	 SetE5x5(Double_t x)                        { fE5x5 = x;            }
    void	 SetSwissCross(Double_t x)                  { fSwissCross = x;      }
    void	 SetE2x5Right(Double_t x)                   { fE2x5Right = x;       }
    void	 SetE2x5Left(Double_t x)                    { fE2x5Left = x;        }
    void	 SetE2x5Top(Double_t x)                     { fE2x5Top = x;         }
    void	 SetE2x5Bottom(Double_t x)                  { fE2x5Bottom = x;      }
    void	 SetE2x5Max(Double_t x)                     { fE2x5Max = x;         }
    void	 SetELeft(Double_t x)                       { fELeft = x;           }
    void	 SetERight(Double_t x)                      { fERight = x;          }
    void	 SetETop(Double_t x)                        { fETop = x;            }
    void	 SetEBottom(Double_t x)                     { fEBottom = x;         }
    void	 SetEMax(Double_t x)                        { fEMax = x;            }
    void	 SetE2nd(Double_t x)                        { fE2nd = x;            }
    void	 SetEtaLat(Double_t x)                      { fEtaLat = x;          }
    void	 SetPhiLat(Double_t x)                      { fPhiLat = x;          }
    void	 SetLat(Double_t x)                         { fLat = x;             }
    void	 SetCovEtaEta(Double_t x)                   { fCovEtaEta = x;       }
    void	 SetCovEtaPhi(Double_t x)                   { fCovEtaPhi = x;       }
    void	 SetCovPhiPhi(Double_t x)                   { fCovPhiPhi = x;       }
    void	 SetCoviEtaiEta(Double_t x)                 { fCoviEtaiEta = x;     }
    void	 SetCoviEtaiPhi(Double_t x)                 { fCoviEtaiPhi = x;     }
    void	 SetCoviPhiiPhi(Double_t x)                 { fCoviPhiiPhi = x;     }
    void	 SetZernike20(Double_t x)                   { fZernike20 = x;       }
    void	 SetZernike42(Double_t x)                   { fZernike42 = x;       }
    void         SetEtaCry(Double_t x)                      { fEtaCry = x;          }
    void         SetPhiCry(Double_t x)                      { fPhiCry = x;          }
    void         SetXCry(Double_t x)                        { fXCry = x;            }
    void         SetYCry(Double_t x)                        { fYCry = x;            }
    void         SetThetaAxis(Double_t x)                   { fThetaAxis = x;       }
    void         SetPhiAxis(Double_t x)                     { fPhiAxis = x;         }
    void         SetIEta(Short_t i)                         { fIEta = i;            }
    void         SetIPhi(Short_t i)                         { fIPhi = i;            }
    void         SetIX(Short_t i)                           { fIX = i;              }
    void         SetIY(Short_t i)                           { fIY = i;              }
    void         SetSeedIEta(Short_t i)                     { fSeedIEta = i;        }
    void         SetSeedIPhi(Short_t i)                     { fSeedIPhi = i;        }
    void         SetSeedIX(Short_t i)                       { fSeedIX = i;          }
    void         SetSeedIY(Short_t i)                       { fSeedIY = i;          }
    void         SetInsideMustache(Bool_t b)                { fInsideMustache = b;  }
    void         SetTime(Double_t x)                        { fTime = x;            }
    void         SetSeedTime(Double_t x)                    { fSeedTime = x;        }
    void         SetMatchedEnergy(Double_t x)               { fMatchedEnergy = x;   }
    
  protected:
    Double32_t   fEnergy;  //[0,0,14]assigned energy
    Vect3C       fPoint;   //centroid Position
    
    Int_t        fNHits;
    Double32_t   fE1x3;
    Double32_t   fE3x1;
    Double32_t   fE1x5;
    Double32_t   fE2x2;
    Double32_t   fE3x2;
    Double32_t   fE3x3;
    Double32_t   fE4x4;
    Double32_t   fE5x5;
    Double32_t   fSwissCross;
    Double32_t   fE2x5Right;
    Double32_t   fE2x5Left;
    Double32_t   fE2x5Top;
    Double32_t   fE2x5Bottom;
    Double32_t   fE2x5Max;
    Double32_t   fELeft;
    Double32_t   fERight;
    Double32_t   fETop;
    Double32_t   fEBottom;
    Double32_t   fEMax;
    Double32_t   fE2nd;
    Double32_t   fEtaLat;
    Double32_t   fPhiLat;
    Double32_t   fLat;
    Double32_t   fCovEtaEta;
    Double32_t   fCovEtaPhi;
    Double32_t   fCovPhiPhi;
    Double32_t   fCoviEtaiEta;
    Double32_t   fCoviEtaiPhi;
    Double32_t   fCoviPhiiPhi;
    Double32_t   fZernike20;
    Double32_t   fZernike42;
    Double32_t   fEtaCry;  //local coord. and idx of max energy crystal (local containment corr.)
    Double32_t   fPhiCry;
    Double32_t   fXCry;
    Double32_t   fYCry;
    Double32_t   fThetaAxis;
    Double32_t   fPhiAxis;
    Short_t      fIEta;
    Short_t      fIPhi;
    Short_t      fIX;
    Short_t      fIY;
    Short_t      fSeedIEta;
    Short_t      fSeedIPhi;
    Short_t      fSeedIX;
    Short_t      fSeedIY;
    Bool_t       fInsideMustache; //is inside mustache profile of parent supercluster
    Double32_t   fTime;           //ecal timing (weighted average)
    Double32_t   fSeedTime;       //ecal timing (seed crystal)
    Double32_t   fMatchedEnergy;  //energy matched to egamma supercluster footprint
    
    ClassDef(BasicCluster, 6)     // Basic cluster class
  };
}

//--------------------------------------------------------------------------------------------------
inline Double_t mithep::BasicCluster::Et() const
{
  // Return transverse energy.

  return fEnergy*fPoint.Rho()/fPoint.V().R();
}
#endif
