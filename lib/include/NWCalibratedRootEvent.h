#ifndef NWCALIBRATEDROOTEVENT_H
#define NWCALIBRATEDROOTEVENT_H

#include <TROOT.h>

class NeutronWallCalibratedData
{
public:
  Int_t    fmulti;
  Int_t    *fnumbar;      //[fmulti]
  Short_t  *fLeft;        //[fmulti]
  Short_t  *fRight;       //[fmulti]
  Short_t  *ffastLeft;    //[fmulti]
  Short_t  *ffastRight;   //[fmulti]
  Double_t *fTimeLeft;    //[fmulti]
  Double_t *fTimeRight;   //[fmulti]
  // Double_t *fGeoMean;     //[fmulti]
  // Double_t *ffastGeoMean; //[fmulti]

  // Double_t *fGeoMeanSaturationCorrected; //[fmulti]
  // Double_t *fTimeMean;    //[fmulti]
  // Double_t *fCalGeoMean;  //[fmulti]
  // Double_t *fMatchedLeft; //[fmulti]
  // Double_t *fMatchedRight;//[fmulti]
  // Double_t *fMatchedGeoMean;//[fmulti]
  // Double_t *fFlattenedGeoMean;//[fmulti]
  // Double_t *fLeftSaturationCorrected;//[fmulti]
  // Double_t *fRightSaturationCorrected;//[fmulti]
  // Double_t *ffastLeftSaturationCorrected;//[fmulti]
  // Double_t *ffastRightSaturationCorrected;//[fmulti]
  // Double_t *fPSDLeft;     //[fmulti]
  // Double_t *fPSDRight;    //[fmulti]
  // Double_t *fTheta;       //[fmulti]
  // Double_t *fPhi;         //[fmulti]
  // Double_t *fThetaRan;    //[fmulti]
  // Double_t *fPhiRan;      //[fmulti]

  // Double_t *fDistcm;      //[fmulti]
  // Double_t *fDistRancm;   //[fmulti]
  // Double_t *fXcm;         //[fmulti]
  // Bool_t   *fIsGamma;     //[fmulti]

  NeutronWallCalibratedData(int);
  NeutronWallCalibratedData();
  virtual ~NeutronWallCalibratedData();

  ClassDef(NeutronWallCalibratedData,1);
};

class VetoWallCalibratedData
{
public:
  Int_t    fmulti;
  Int_t    *fnumbar;      //[fmulti]
  Short_t  *fTop;         //[fmulti]
  Short_t  *fBottom;      //[fmulti]
  Double_t *fTimeTop;     //[fmulti]
  Double_t *fTimeBottom;  //[fmulti]
  // Double_t *fGeoMean;     //[fmulti]

  // Double_t *fGeoMeanMatched; //[fmulti]
  // Double_t *fYcm;            //[fmulti]
  // Double_t *fTheta;          //[fmulti]
  // Double_t *fPhi;            //[fmulti]
  // Double_t *fThetaRan;       //[fmulti]
  // Double_t *fPhiRan;         //[fmulti]
  // Double_t *fDistcm;         //[fmulti]
  // Double_t *fDistRancm;      //[fmulti]

  VetoWallCalibratedData(int);
  VetoWallCalibratedData();
  virtual ~VetoWallCalibratedData();

  ClassDef(VetoWallCalibratedData,1);
};

class ForwardArrayCalibratedData
{
public:
  Int_t    fmulti;
  Int_t    *fnumdet;      //[fmulti]
  Short_t  *fE;           //[fmulti]
  Double_t *fTime;        //[fmulti]

  Double_t fTimeMean;
  Double_t fTimeMin;


  ForwardArrayCalibratedData(int);
  ForwardArrayCalibratedData();
  virtual ~ForwardArrayCalibratedData();

  ClassDef(ForwardArrayCalibratedData,2);
};

#endif
