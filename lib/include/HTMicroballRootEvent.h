#ifndef HTMICROBALLROOTEVENT_H
#define HTMICROBALLROOTEVENT_H

#include <TROOT.h>

#include "shared.h"

class HTMicroballData
{
public:
  Int_t    fmulti;
  Int_t    *fnumring;     //[fmulti]
  Int_t    *fnumdet;      //[fmulti]
  Short_t  *fTail;        //[fmulti]
  Short_t  *fFast;        //[fmulti]
  Short_t  *fTime;        //[fmulti]

  HTMicroballData();
  HTMicroballData(int);
  virtual ~HTMicroballData();

  ClassDef(HTMicroballData,1);
};

#endif
