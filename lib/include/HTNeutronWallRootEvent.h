#ifndef HTNEUTRONWALLROOTEVENT_H
#define HTNEUTRONWALLROOTEVENT_H

#include <TROOT.h>

class HTNeutronWallData
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
  Double_t *fGeoMean;     //[fmulti]
  Double_t *ffastGeoMean; //[fmulti]


  HTNeutronWallData(int);
  HTNeutronWallData();
  virtual ~HTNeutronWallData();

  ClassDef(HTNeutronWallData,1);
};

class HTNeutronWallRootEvent
{
 private:
 int fNumBars;

 public:
 HTNeutronWallData fNeutronWall;

 HTNeutronWallRootEvent(int);
 virtual ~HTNeutronWallRootEvent();

 ClassDef(HTNeutronWallRootEvent,1);
};

#endif
