#include "include/HiRADetectorStatus.h"

//____________________________________________________
HiRADetectorStatus::HiRADetectorStatus(int TotNumTel, int TotNumStripf, int TotNumStripb) :
fNumTel(TotNumTel),
fNumStripf(TotNumStripf),
fNumStripb(TotNumStripb),
fStripfBad(new bool *[fNumTel]),
fStripbBad(new bool *[fNumTel]),
fStripBadLoaded(false)
{
  for(int TelNum=0; TelNum<fNumTel; TelNum++) {
    fStripfBad[TelNum]= new bool[fNumStripf];
    fStripbBad[TelNum]= new bool[fNumStripb];
  }
}

//____________________________________________________
HiRADetectorStatus::~HiRADetectorStatus()
{
  for(int TelNum=0; TelNum<fNumTel; TelNum++) {
    delete [] fStripfBad[TelNum];
    delete [] fStripbBad[TelNum];
  }
  delete [] fStripfBad;
  delete [] fStripbBad;
}

//____________________________________________________
int HiRADetectorStatus::LoadStipBad(const char * file_name)
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
    int striptype;
    int numstrip;
    int isbad;

    LineStream >> striptype >> numtel >> numstrip >> isbad;

    if(striptype==1) {
      fStripbBad[numtel][numstrip]=bool(isbad);
    } else if(striptype==0) {
      fStripfBad[numtel][numstrip]=bool(isbad);
    }

    NRead++;
  }

  fStripBadLoaded=true;
  return NRead;
}

//____________________________________________________
bool HiRADetectorStatus::IsStripfBad(int num_tel, int num_strip) const
{
  return fStripBadLoaded ? fStripfBad[num_tel][num_strip] : false;
}

//____________________________________________________
bool HiRADetectorStatus::IsStripbBad(int num_tel, int num_strip) const
{
  return fStripBadLoaded ? fStripbBad[num_tel][num_strip] : false;
}
