#include "include/MBDetectorStatus.h"

//____________________________________________________
MBDetectorStatus::MBDetectorStatus() :
fBadDetectors(0)
{
  fBadDetectors = new bool * [N_MICROBALL_RINGS];
  for(int i=0; i< N_MICROBALL_RINGS; i++) {
    fBadDetectors[i] = new bool [N_MICROBALL_DETS_PER_RING[i]];
  }
}

//____________________________________________________
MBDetectorStatus::~MBDetectorStatus()
{
  if(fBadDetectors) {
    for(int i=0; i< N_MICROBALL_RINGS; i++) {
      if(fBadDetectors[i]) delete [] fBadDetectors[i];
    }
    delete [] fBadDetectors;
  }
}

//____________________________________________________
int MBDetectorStatus::LoadBadDetectors(const char * file_name)
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

    std::string DetName;
    bool status;

    LineStream >> DetName >> status;

    int ring = std::stoi(DetName.substr(DetName.find("R")+1,2));
    int detector = std::stoi(DetName.substr(DetName.find("_")+1,2));

    fBadDetectors[ring-1][detector-1] = status;

    NRead++;
  }

  return NRead;
}

//____________________________________________________
bool MBDetectorStatus::IsBad(int num_ring, int num_det) const
{
  return fBadDetectors[num_ring-1][num_det-1];
}
