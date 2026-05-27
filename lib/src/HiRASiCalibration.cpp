#include "include/HiRASiCalibration.h"

//____________________________________________________
HiRASiCalibration::HiRASiCalibration(int TotNumTel, int TotNumStripf, int TotNumStripb) :
fNumTel(TotNumTel),
fNumStripf(TotNumStripf),
fNumStripb(TotNumStripb),
fIntercept(new double[fNumTel*fNumStripf+fNumTel*fNumStripb]),
fSlope(new double[fNumTel*fNumStripf+fNumTel*fNumStripb]),
fMatchingIntercept(new double[fNumTel*fNumStripf+fNumTel*fNumStripb]),
fMatchingSlope(new double[fNumTel*fNumStripf+fNumTel*fNumStripb]),
fMatchingSaturationPoint(new double[fNumTel*fNumStripf+fNumTel*fNumStripb]),
fCalibrationLoaded(false),
fMatchingLoaded(false)
{}

//____________________________________________________
HiRASiCalibration::~HiRASiCalibration()
{
  delete [] fIntercept;
  delete [] fSlope;
  delete [] fMatchingIntercept;
  delete [] fMatchingSlope;
  delete [] fMatchingSaturationPoint;
}

//____________________________________________________
int HiRASiCalibration::LoadCalibration(const char * file_name)
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
    int numstrip;
    bool FB;

    LineStream >> FB >> numtel >> numstrip;
    LineStream >> fIntercept[FB*fNumTel*fNumStripf+numtel*(FB?fNumStripb:fNumStripf)+numstrip]
    >> fSlope[FB*fNumTel*fNumStripf+numtel*(FB?fNumStripb:fNumStripf)+numstrip];

    NRead++;
  }

  fCalibrationLoaded=true;
  return NRead;
}

//____________________________________________________
int HiRASiCalibration::LoadMatching(const char * file_name)
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
    int numstrip;
    bool FB;

    LineStream  >> numtel >> numstrip >> FB;
    LineStream >> fMatchingIntercept[FB*fNumTel*fNumStripf+numtel*(FB?fNumStripb:fNumStripf)+numstrip]
    >> fMatchingSlope[FB*fNumTel*fNumStripf+numtel*(FB?fNumStripb:fNumStripf)+numstrip]>>fMatchingSaturationPoint[FB*fNumTel*fNumStripf+numtel*(FB?fNumStripb:fNumStripf)+numstrip];

    NRead++;
  }

  fMatchingLoaded=true;
  return NRead;
}

//____________________________________________________
double HiRASiCalibration::GetEnergy(double ch, int num_tel, int num_strip, bool FrontBack) const
{
  return fCalibrationLoaded ? ch*fSlope[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip]
                                +fIntercept[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip] : -9999;
}

//____________________________________________________
double HiRASiCalibration::GetSlope(int num_tel, int num_strip, bool FrontBack) const
{
  return fCalibrationLoaded ? fSlope[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip] : -9999;
}

//____________________________________________________
double HiRASiCalibration::GetIntercept(int num_tel, int num_strip, bool FrontBack) const
{
  return fCalibrationLoaded ? fIntercept[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip] : -9999;
}

//____________________________________________________
double HiRASiCalibration::GetHiLowMatched(double chHi, double chLow, int num_tel, int num_strip, bool FrontBack) const
{
  if(!fMatchingLoaded) return -9999;
  if(chLow<fMatchingSaturationPoint[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip]) {
    return chHi;
  }
  else {
    return chLow*fMatchingSlope[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip]
    +fMatchingIntercept[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip];
  }
}

//____________________________________________________
double HiRASiCalibration::GetEnergyHiLowMatched(double chHi, double chLow, int num_tel, int num_strip, bool FrontBack) const
{
  return fMatchingLoaded ? GetHiLowMatched(chHi,chLow,num_tel,num_strip,FrontBack)*fSlope[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip]
                                +fIntercept[FrontBack*fNumTel*fNumStripf+num_tel*(FrontBack?fNumStripb:fNumStripf)+num_strip] : -9999;
}
