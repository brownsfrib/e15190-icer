#include "include/HiRACalibratedRootEvent.h"

//________________________________________________
HiRACalibratedData::HiRACalibratedData(int num_max_particles) :
fmulti(0)
{
  fnumtel=           new  Int_t    [num_max_particles];
  fnumstripf=        new  Int_t    [num_max_particles];
  fEnergySifHi=      new  UShort_t [num_max_particles];
  fEnergySifLo=      new  UShort_t [num_max_particles];
  fTimeSif=          new  UShort_t [num_max_particles];
  fnumstripb=        new  Int_t    [num_max_particles];
  fEnergySibHi=      new  UShort_t [num_max_particles];
  fEnergySibLo=      new  UShort_t [num_max_particles];
  fTimeSib=          new  UShort_t [num_max_particles];
  fnumcsi=           new  Int_t    [num_max_particles];
  fEnergycsi=        new  UShort_t [num_max_particles];
  fTimecsi=          new  Double_t [num_max_particles];

  fEnergySifMatched= new  Double_t [num_max_particles];
  fEnergySibMatched= new  Double_t [num_max_particles];
  fEnergySifCal=     new  Double_t [num_max_particles];
  fEnergySibCal=     new  Double_t [num_max_particles];
  fEnergycsiCal=     new  Double_t [num_max_particles];
  fEnergycsiCalProtons=     new  Double_t [num_max_particles];
  fEnergycsiVolt=     new  Double_t [num_max_particles];

  fKinEnergy=	       new  Double_t [num_max_particles];
  fKinEnergyCMS=	   new  Double_t [num_max_particles];
  fMomentum=	       new  Double_t [num_max_particles];
  fMomentumCMS=      new  Double_t [num_max_particles];
  fTheta=	           new  Double_t [num_max_particles];
  fThetaCMS=         new  Double_t [num_max_particles];
  fPhi= 	           new  Double_t [num_max_particles];
  fZ=		             new  Double_t [num_max_particles];
  fA=		             new  Double_t [num_max_particles];
  fZId=	             new  Int_t    [num_max_particles];
  fAId=              new  Int_t    [num_max_particles];
  fIdCode=	         new  Int_t    [num_max_particles];
}

//________________________________________________
HiRACalibratedData::HiRACalibratedData() :
fmulti(0)
{
  fnumtel=           new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fnumstripf=        new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySifHi=      new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySifLo=      new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fTimeSif=          new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fnumstripb=        new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySibHi=      new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySibLo=      new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fTimeSib=          new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fnumcsi=           new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergycsi=        new  UShort_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fTimecsi=          new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];

  fEnergySifMatched= new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySibMatched= new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySifCal=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergySibCal=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergycsiCal=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergycsiCalProtons=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fEnergycsiVolt=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];

  fKinEnergy=	       new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fKinEnergyCMS=     new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fMomentum=	       new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fMomentumCMS=      new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fTheta=	           new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fThetaCMS=         new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fPhi= 	           new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fZ=		             new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fA=		             new  Double_t [12*NUM_MAX_PARTICLES_PER_TEL];
  fZId=	             new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fAId=              new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
  fIdCode=	         new  Int_t    [12*NUM_MAX_PARTICLES_PER_TEL];
}

//________________________________________________
HiRACalibratedData::~HiRACalibratedData()
{
  delete []  fnumtel;
  delete []  fnumstripf;
  delete []  fEnergySifHi;
  delete []  fEnergySifLo;
  delete []  fTimeSif ;
  delete []  fnumstripb;
  delete []  fEnergySibHi;
  delete []  fEnergySibLo;
  delete []  fTimeSib;
  delete []  fnumcsi;
  delete []  fEnergycsi;
  delete []  fTimecsi;

  delete []  fEnergySifMatched;
  delete []  fEnergySibMatched;
  delete []  fEnergySifCal;
  delete []  fEnergySibCal;
  delete []  fEnergycsiCal;
  delete []  fEnergycsiCalProtons;
  delete []  fEnergycsiVolt;

  delete []  fKinEnergy;
  delete []  fKinEnergyCMS;
  delete []  fMomentum;
  delete []  fMomentumCMS;
  delete []  fTheta;
  delete []  fThetaCMS;
  delete []  fPhi;
  delete []  fZId;
  delete []  fAId;
  delete []  fZ;
  delete []  fA;
  delete []  fIdCode;
}

//________________________________________________
HiRACalibratedBuffer::HiRACalibratedBuffer() :
fmultistripf(0),
fmultistripb(0),
fmulticsi(0)
{
  fnumstripf=        new  Int_t    [NUM_STRIP_F];
  fEnergySifHi=      new  UShort_t [NUM_STRIP_F];
  fEnergySifLo=      new  UShort_t [NUM_STRIP_F];
  fTimeSif=          new  UShort_t [NUM_STRIP_F];
  fEnergySifMatched= new  Double_t [NUM_STRIP_F];
  fEnergySifCal=     new  Double_t [NUM_STRIP_F];

  fnumstripb=        new  Int_t    [NUM_STRIP_B];
  fEnergySibHi=      new  UShort_t [NUM_STRIP_B];
  fEnergySibLo=      new  UShort_t [NUM_STRIP_B];
  fTimeSib=          new  UShort_t [NUM_STRIP_B];
  fEnergySibMatched= new  Double_t [NUM_STRIP_B];
  fEnergySibCal=     new  Double_t [NUM_STRIP_B];

  fnumcsi=           new  Int_t    [NUM_CSI_TEL];
  fEnergycsi=        new  UShort_t [NUM_CSI_TEL];
  fTimecsi=          new  Double_t [NUM_CSI_TEL];
  fEnergycsiCalProtons= new  Double_t [NUM_CSI_TEL];
  fEnergycsiVolt=     new  Double_t [NUM_CSI_TEL];
}

//________________________________________________
HiRACalibratedBuffer::~HiRACalibratedBuffer()
{
  delete []  fnumstripf;
  delete []  fEnergySifHi;
  delete []  fEnergySifLo;
  delete []  fTimeSif ;

  delete []  fnumstripb;
  delete []  fEnergySibHi;
  delete []  fEnergySibLo;
  delete []  fTimeSib;

  delete []  fnumcsi;
  delete []  fEnergycsi;
  delete []  fTimecsi;

  delete []  fEnergySifMatched;
  delete []  fEnergySibMatched;
  delete []  fEnergySifCal;
  delete []  fEnergySibCal;
  delete []  fEnergycsiCalProtons;
  delete []  fEnergycsiVolt;
}
