#include "include/HTMicroballRootEvent.h"

//________________________________________________
HTMicroballData::HTMicroballData() :
fmulti(0)
{
  fnumring =new Int_t   [NUM_DETS_MICROBALL];
  fnumdet  =new Int_t   [NUM_DETS_MICROBALL];
  fTail    =new Short_t [NUM_DETS_MICROBALL];
  fFast    =new Short_t [NUM_DETS_MICROBALL];
  fTime    =new Short_t [NUM_DETS_MICROBALL];
}

//________________________________________________
HTMicroballData::HTMicroballData(int num_detectors) :
fmulti(0)
{
  fnumring =new Int_t   [num_detectors];
  fnumdet  =new Int_t   [num_detectors];
  fTail    =new Short_t [num_detectors];
  fFast    =new Short_t [num_detectors];
  fTime    =new Short_t [num_detectors];
}

//________________________________________________
HTMicroballData::~HTMicroballData()
{
  delete [] fnumring;
  delete [] fnumdet ;
  delete [] fTail   ;
  delete [] fFast   ;
  delete [] fTime   ;
}
