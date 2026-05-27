#include <HiRAIdentification.h>

//________________________________________________
HiRAIdentification::HiRAIdentification() :
fpar(0),
fIdentificationLoaded(false),
fNumLines(0)
{}

//________________________________________________
HiRAIdentification::~HiRAIdentification()
{
  if(fpar) {
    for(int i=0; i<fNumLines; i++) {
      delete fpar[i];
    }
    delete [] fpar;
  }
}

//________________________________________________
int HiRAIdentification::LoadIdentification(const char * name)
{
  ifstream ifile(name);
  if (!ifile.is_open()){
    return -1;
  }
  ifile >> fNumLines;
  fpar = new ZApar*[fNumLines];
  for (int i=0;i<fNumLines;i++)
  {
    fpar[i] = new ZApar(ifile);
  }

  ifile.close();

  fIdentificationLoaded=true;

  return fNumLines;
}

//________________________________________________
int HiRAIdentification::PerformIdentification(HiRACalibratedData * pCalibratedEvent)
{
  int NIdParticles=0;
  for(int i=0; i<pCalibratedEvent->fmulti; i++)
  {
    pCalibratedEvent->fZ[i]=-9999;
    pCalibratedEvent->fA[i]=-9999;
    pCalibratedEvent->fIdCode[i]=0;
    for(int line=0; line<fNumLines; line++) {
      if(fIdentificationLoaded && fpar[line]->inBanana(pCalibratedEvent->fEnergycsiCal[i],pCalibratedEvent->fEnergySifCal[i])) {
        pCalibratedEvent->fZ[i]=fpar[line]->Z;
        pCalibratedEvent->fA[i]=fpar[line]->A;
        pCalibratedEvent->fIdCode[i]=1;
        NIdParticles++;
        break;
      }
    }
  }

  return NIdParticles;
}

//________________________________________________
int HiRAIdentification::GetIdentifiedZ(double ERes, double DE)
{
  for(int line=0; line<fNumLines; line++) {
    if(fIdentificationLoaded && fpar[line]->inBanana(ERes,DE)) {
      return fpar[line]->Z;
    }
  }

  return -1;
}

//________________________________________________
int HiRAIdentification::GetIdentifiedA(double ERes, double DE)
{
  for(int line=0; line<fNumLines; line++) {
    if(fIdentificationLoaded && fpar[line]->inBanana(ERes,DE)) {
      return fpar[line]->A;
    }
  }

  return -1;
}
