#ifndef HIRACALIBRATEDROOTEVENT_H
#define HIRACALIBRATEDROOTEVENT_H

#include <TROOT.h>

#include "shared.h"

class HiRACalibratedData
{
public:
  Int_t    fmulti;
  Int_t    *fnumtel;       //[fmulti]
  Int_t    *fnumstripf;    //[fmulti]
  UShort_t *fEnergySifHi;  //[fmulti]
  UShort_t *fEnergySifLo;  //[fmulti]
  UShort_t *fTimeSif;      //[fmulti]
  Int_t    *fnumstripb;    //[fmulti]
  UShort_t *fEnergySibHi;  //[fmulti]
  UShort_t *fEnergySibLo;  //[fmulti]
  UShort_t *fTimeSib;      //[fmulti]
  Int_t    *fnumcsi;       //[fmulti]
  UShort_t *fEnergycsi;    //[fmulti]
  Double_t *fTimecsi;      //[fmulti]

  Double_t *fEnergySifMatched; //[fmulti]
  Double_t *fEnergySibMatched; //[fmulti]
  Double_t *fEnergySifCal;     //[fmulti]
  Double_t *fEnergySibCal;     //[fmulti]
  Double_t *fEnergycsiCal;     //[fmulti]
  Double_t *fEnergycsiCalProtons; //[fmulti]
  Double_t *fEnergycsiVolt;     //[fmulti]

  Double_t *fKinEnergy;        //[fmulti]
  Double_t *fKinEnergyCMS;        //[fmulti]
  Double_t *fMomentum;         //[fmulti]
  Double_t *fMomentumCMS;         //[fmulti]
  Double_t *fTheta;            //[fmulti]
  Double_t *fThetaCMS;            //[fmulti]
  Double_t *fPhi;              //[fmulti]
  Double_t *fZ;                //[fmulti]
  Double_t *fA;                //[fmulti]
  Int_t    *fZId;              //[fmulti]
  Int_t    *fAId;              //[fmulti]
  Int_t    *fIdCode;           //[fmulti]

  HiRACalibratedData(int);
  HiRACalibratedData();
  virtual ~HiRACalibratedData();

  ClassDef(HiRACalibratedData,1);
};

class HiRACalibratedBuffer
{
public:
  Int_t    fmultistripf;
  Int_t    *fnumstripf;    //[fmultistripf]
  UShort_t *fEnergySifHi;  //[fmultistripf]
  UShort_t *fEnergySifLo;  //[fmultistripf]
  UShort_t *fTimeSif;      //[fmultistripf]
  Double_t *fEnergySifMatched; //[fmultistripf]
  Double_t *fEnergySifCal;     //[fmultistripf]

  Int_t    fmultistripb;
  Int_t    *fnumstripb;    //[fmultistripb]
  UShort_t *fEnergySibHi;  //[fmultistripb]
  UShort_t *fEnergySibLo;  //[fmultistripb]
  UShort_t *fTimeSib;      //[fmultistripb]
  Double_t *fEnergySibMatched; //[fmultistripb]
  Double_t *fEnergySibCal;     //[fmultistripb]

  Int_t    fmulticsi;
  Int_t    *fnumcsi;       //[fmulticsi]
  UShort_t *fEnergycsi;    //[fmulticsi]
  Double_t *fTimecsi;      //[fmulticsi]
  Double_t *fEnergycsiCalProtons;  //[fmulticsi]
  Double_t *fEnergycsiVolt;     //[fmulticsi]
  Double_t *fEnergycsiCal;

  HiRACalibratedBuffer();
  virtual ~HiRACalibratedBuffer();

  ClassDef(HiRACalibratedBuffer,1);
};

#endif
