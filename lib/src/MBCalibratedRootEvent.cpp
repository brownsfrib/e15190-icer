#include <MBCalibratedRootEvent.h>

//________________________________________________
MicroballCalibratedData::MicroballCalibratedData() :
fmulti(0)
{
  fnumring 	= new Int_t   [NUM_DETS_MICROBALL];
  fnumdet  	= new Int_t   [NUM_DETS_MICROBALL];
  fTail    	= new Short_t [NUM_DETS_MICROBALL];
  fFast    	= new Short_t [NUM_DETS_MICROBALL];
  fTime    	= new Short_t [NUM_DETS_MICROBALL];

  fTheta   	= new Double_t[NUM_DETS_MICROBALL];
  fPhi     	= new Double_t[NUM_DETS_MICROBALL];
  fThetaRan	= new Double_t[NUM_DETS_MICROBALL];
  fPhiRan  	= new Double_t[NUM_DETS_MICROBALL]; 
}

//________________________________________________
MicroballCalibratedData::MicroballCalibratedData(int num_dets) :
fmulti(0)
{
  fnumring 	= new Int_t   [num_dets];
  fnumdet  	= new Int_t   [num_dets];
  fTail    	= new Short_t [num_dets];
  fFast    	= new Short_t [num_dets];
  fTime    	= new Short_t [num_dets];

  fTheta   	= new Double_t[num_dets];
  fPhi     	= new Double_t[num_dets];
}

//________________________________________________
MicroballCalibratedData::~MicroballCalibratedData()
{
  delete [] fnumring;
  delete [] fnumdet ;
  delete [] fTail   ;
  delete [] fFast   ;
  delete [] fTime   ;

  delete [] fTheta;
  delete [] fPhi;
}
