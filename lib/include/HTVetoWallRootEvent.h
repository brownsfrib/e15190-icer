#ifndef HTVETOWALLROOTEVENT_H
#define HTVETOWALLROOTEVENT_H

#include <TROOT.h>

class HTVetoWallData
{
public:
  Int_t    fmulti;
  Int_t    *fnumbar;     //[fmulti]
  Short_t  *fTop;        //[fmulti]
  Short_t  *fBottom;     //[fmulti]
  Double_t *fTimeTop;    //[fmulti]
  Double_t *fTimeBottom; //[fmulti]
  Double_t *fGeoMean;    //[fmulti]

  HTVetoWallData(int);
  HTVetoWallData();
  virtual ~HTVetoWallData();

  ClassDef(HTVetoWallData,1);
};

class HTVetoWallRootEvent
{
 private:
 int fNumBars;

 public:
 HTVetoWallData fVetoWall;

 HTVetoWallRootEvent(int);
 virtual ~HTVetoWallRootEvent();

 ClassDef(HTVetoWallRootEvent,1);
};

#endif
