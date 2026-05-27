#ifndef HTHIRAROOTEVENT_H
#define HTHIRAROOTEVENT_H

#include <TROOT.h>

class HTHiRAStripData
{
public:
  Int_t    fmulti;
  Int_t    *fnumstrip;   //[fmulti]
  UShort_t *fEnergyHi;   //[fmulti]
  UShort_t *fEnergyLo;   //[fmulti]
  UShort_t *fTime;       //[fmulti]

  HTHiRAStripData();
  virtual ~HTHiRAStripData();

  ClassDef(HTHiRAStripData,1);
};

class HTHiRACsIData
{
public:
  Int_t    fmulti;
  Int_t    *fnumcsi;     //[fmulti]
  UShort_t *fEnergy;     //[fmulti]
  Double_t *fTime;       //[fmulti]

  HTHiRACsIData();
  virtual ~HTHiRACsIData();

  ClassDef(HTHiRACsIData,1);
};

class HTHiRAData
{
public:
  HTHiRAStripData fDE;
  HTHiRAStripData fEF;
  HTHiRAStripData fEB;
  HTHiRACsIData   fCsI;

  HTHiRAData();
  virtual ~HTHiRAData();

  ClassDef(HTHiRAData,1);
};

class HTHiRARootEvent
{
 private:
 int fNumTelescopes;

 public:
 HTHiRAData *fHiRA;

 HTHiRARootEvent(int);
 virtual ~HTHiRARootEvent();

 ClassDef(HTHiRARootEvent,1);
};

#endif
