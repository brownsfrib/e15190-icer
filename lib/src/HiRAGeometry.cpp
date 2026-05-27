#include "include/HiRAGeometry.h"

//____________________________________________________
HiRAGeometry::HiRAGeometry(int TotNumTel, int TotNumStripf, int TotNumStripb) :
fNumTel(TotNumTel),
fNumStripf(TotNumStripf),
fNumStripb(TotNumStripb),
fRadToDeg(180./TMath::Pi()),
fTheta(new double *[fNumTel]),
fPhi(new double *[fNumTel]),
fThetaDeg(new double *[fNumTel]),
fPhiDeg(new double *[fNumTel]),
fPixelsLoaded(false)
{
  for(int i=0; i<fNumTel; i++) {
    fTheta[i]= new double[fNumStripf*fNumStripb];
    fPhi[i]= new double[fNumStripf*fNumStripb];
    fThetaDeg[i]= new double[fNumStripf*fNumStripb];
    fPhiDeg[i]= new double[fNumStripf*fNumStripb];
  }
}

//____________________________________________________
HiRAGeometry::~HiRAGeometry()
{
  for(int i=0; i<fNumTel; i++) {
    delete [] fTheta[i];
    delete [] fPhi[i];
    delete [] fThetaDeg[i];
    delete [] fPhiDeg[i];
  }
  delete [] fTheta;
  delete [] fPhi;
  delete [] fThetaDeg;
  delete [] fPhiDeg;
}

//____________________________________________________
int HiRAGeometry::LoadPixels(const char * file_name)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    return -1;
  }
  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);

    if(LineRead.empty()) continue;
    if(LineRead.find('*')==0) continue;
    if(LineRead.find_first_not_of(' ') == std::string::npos) continue;

    std::istringstream LineStream(LineRead);

    int numtel;
    int numstripf;
    int numstripb;
    double theta;
    double phi;

    LineStream >> numtel >> numstripf >> numstripb >> theta >> phi;

    //NOTE: Theta and Phi are given in deg
    fThetaDeg[numtel][fNumStripf*numstripf+numstripb]=theta;
    fPhiDeg  [numtel][fNumStripf*numstripf+numstripb]=phi;
    fTheta   [numtel][fNumStripf*numstripf+numstripb]=theta/fRadToDeg;
    fPhi     [numtel][fNumStripf*numstripf+numstripb]=phi/fRadToDeg;

    NRead++;
  }

  fPixelsLoaded=true;
  return NRead;
}

//____________________________________________________
double HiRAGeometry::GetThetaPixel(int num_tel, int stripf, int stripb) const
{
  return fPixelsLoaded ? fTheta[num_tel][fNumStripf*stripf+stripb] : -9999;
}

//____________________________________________________
double HiRAGeometry::GetPhiPixel(int num_tel, int stripf, int stripb) const
{
  return fPixelsLoaded ? fPhi[num_tel][fNumStripf*stripf+stripb] : -9999;
}

//____________________________________________________
double HiRAGeometry::GetThetaPixelDeg(int num_tel, int stripf, int stripb) const
{
  return fPixelsLoaded ? fThetaDeg[num_tel][fNumStripf*stripf+stripb] : -9999;
}

//____________________________________________________
double HiRAGeometry::GetPhiPixelDeg(int num_tel, int stripf, int stripb) const
{
  return fPixelsLoaded ? fPhiDeg[num_tel][fNumStripf*stripf+stripb] : -9999;
}
