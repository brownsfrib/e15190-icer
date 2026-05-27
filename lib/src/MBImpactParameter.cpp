#include "include/MBImpactParameter.h"

//____________________________________________________
MBImpactParameter::MBImpactParameter() :
fImpactParameters(new double [NUM_DETS_MICROBALL]),
fImpactParameterErrors(new double [NUM_DETS_MICROBALL]),
fbhat(new double [NUM_DETS_MICROBALL]),
fbhatErrors(new double [NUM_DETS_MICROBALL]),
fImpactParameterLoaded(false)
{
  for(int i=0; i<NUM_DETS_MICROBALL; i++) {
    fImpactParameters[i]=0;
    fImpactParameterErrors[i]=0;
    fbhat[i]=0;
    fbhatErrors[i]=0;
  }
}

//____________________________________________________
MBImpactParameter::~MBImpactParameter()
{
  delete [] fImpactParameters;
  delete [] fImpactParameterErrors;
  delete [] fbhat;
  delete [] fbhatErrors;
}

//____________________________________________________
int MBImpactParameter::LoadImpactParameter(const char * file_name)
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
    LineRead.assign(LineRead.substr(0,LineRead.find('*')));
    if(LineRead.find_first_not_of(' ') == std::string::npos) continue;

    std::istringstream LineStream(LineRead);

    int multi;

    LineStream>>multi;
    LineStream>>fbhat[multi-1]>>fbhatErrors[multi-1]>>fImpactParameters[multi-1]>>fImpactParameterErrors[multi-1];

    NRead++;
  }

  fImpactParameterLoaded=true;
  return NRead;
}

//____________________________________________________
double MBImpactParameter::GetImpactParameter(int multiplicity) const
{
  return fImpactParameterLoaded>0 && multiplicity>0 ? fImpactParameters[multiplicity-1] : -9999;
}

//____________________________________________________
double MBImpactParameter::GetImpactParameterError(int multiplicity) const
{
  return fImpactParameterLoaded>0 && multiplicity>0 ? fImpactParameterErrors[multiplicity-1] : -9999;
}

//____________________________________________________
double MBImpactParameter::Getbhat(int multiplicity) const
{
  return fImpactParameterLoaded>0 && multiplicity>0 ? fbhat[multiplicity-1] : -9999;
}

//____________________________________________________
double MBImpactParameter::GetbhatError(int multiplicity) const
{
  return fImpactParameterLoaded>0 && multiplicity>0 ? fbhatErrors[multiplicity-1] : -9999;
}
