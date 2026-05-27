#ifndef MBCALIBRATEDROOTEVENT_H
#define MBCALIBRATEDROOTEVENT_H

#include <TROOT.h>

#include "shared.h"

class MicroballCalibratedData
{
public:
  Int_t    fmulti;
  Int_t    *fnumring;     //[fmulti]
  Int_t    *fnumdet;      //[fmulti]
  Short_t  *fTail;        //[fmulti]
  Short_t  *fFast;        //[fmulti]
  Short_t  *fTime;        //[fmulti]

  Double_t *fTheta;       //[fmulti]
  Double_t *fPhi;         //[fmulti]
  Double_t *fThetaRan;    //[fmulti]
  Double_t *fPhiRan;      //[fmulti]  

  Double_t fb;
  Double_t fbhat;

  MicroballCalibratedData(int);
  MicroballCalibratedData();
  virtual ~MicroballCalibratedData();

  // 7/25/24 - Sam updated the checksum to 2 because he kept getting an invalid pointer
  // And I'm reaallllyyyy hoping this works
  ClassDef(MicroballCalibratedData, 1);
};

#endif
