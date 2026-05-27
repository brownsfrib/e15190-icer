#ifndef HTFORWARDARRAYROOTEVENT_H
#define HTFORWARDARRAYROOTEVENT_H

#include <TROOT.h>

class HTForwardArrayData
{
public:
  Int_t    fmulti;
  Int_t    *fnumdet;      //[fmulti]
  Short_t  *fE;           //[fmulti]
  Double_t *fTime;        //[fmulti]

  HTForwardArrayData(int);
  HTForwardArrayData();
  virtual ~HTForwardArrayData();

  ClassDef(HTForwardArrayData,1);
};

class HTForwardArrayRootEvent
{
 private:
 int fNumDetectors;

 public:
 HTForwardArrayData fForwardArray;

 HTForwardArrayRootEvent(int);
 virtual ~HTForwardArrayRootEvent();

 ClassDef(HTForwardArrayRootEvent,1);
};

#endif
