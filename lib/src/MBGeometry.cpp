#include "include/MBGeometry.h"

//____________________________________________________
MBGeometry::MBGeometry() :
fThetaMean(0),
fThetaMax(0),
fThetaMin(0),
fPhiMean(0),
fPhiMin(0),
fPhiMax(0),
fRandTheta(0),
fDegToRad(TMath::Pi()/180.),
fRadToDeg(180./TMath::Pi())
{
  fThetaMean = new double * [N_MICROBALL_RINGS];
  fThetaMax = new double * [N_MICROBALL_RINGS];
  fThetaMin = new double * [N_MICROBALL_RINGS];
  fPhiMean = new double * [N_MICROBALL_RINGS];
  fPhiMin = new double * [N_MICROBALL_RINGS];
  fPhiMax = new double * [N_MICROBALL_RINGS];
  fDeltaOmega = new double * [N_MICROBALL_RINGS];
  fDistance = new double * [N_MICROBALL_RINGS];
  for(int i=0; i< N_MICROBALL_RINGS; i++) {
    fThetaMean[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fThetaMax[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fThetaMin[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fPhiMean[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fPhiMin[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fPhiMax[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fDeltaOmega[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
    fDistance[i] = new double [N_MICROBALL_DETS_PER_RING[i]];
  }
  fRandTheta = new TF1("fRandTheta", "sin(x)", 0, TMath::Pi());
}

//____________________________________________________
MBGeometry::~MBGeometry()
{
  for(int i=0; i< N_MICROBALL_RINGS; i++) {
    delete [] fThetaMean[i];
    delete [] fThetaMax[i];
    delete [] fThetaMin[i];
    delete [] fPhiMean[i];
    delete [] fPhiMin[i];
    delete [] fPhiMax[i];
    delete [] fDeltaOmega[i];
    delete [] fDistance[i];
  }
  delete [] fThetaMean;
  delete [] fThetaMax;
  delete [] fThetaMin;
  delete [] fPhiMean;
  delete [] fPhiMin;
  delete [] fPhiMax;
  delete [] fDeltaOmega;
  delete [] fDistance;
}

//____________________________________________________
int MBGeometry::LoadGeometry(const char * file_name)
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
    double theta;
    double half_theta;
    double deltaOmega;
    double distance;

    LineStream >> DetName >> theta >> half_theta >> deltaOmega >> distance;

    int ring = std::stoi(DetName.substr(DetName.find("R")+1,2));
    int detector = std::stoi(DetName.substr(DetName.find("_")+1,2));

    fThetaMean[ring-1][detector-1]=theta*fDegToRad;
    fThetaMin[ring-1][detector-1]=(theta-half_theta)*fDegToRad;
    fThetaMax[ring-1][detector-1]=(theta+half_theta)*fDegToRad;
    fPhiMean[ring-1][detector-1]=(2*TMath::Pi()/N_MICROBALL_DETS_PER_RING[ring-1])*(detector-1);
    fPhiMin[ring-1][detector-1]=(2*TMath::Pi()/N_MICROBALL_DETS_PER_RING[ring-1])*(detector-1)-TMath::Pi()/N_MICROBALL_DETS_PER_RING[ring-1];
    fPhiMax[ring-1][detector-1]=(2*TMath::Pi()/N_MICROBALL_DETS_PER_RING[ring-1])*(detector-1)+TMath::Pi()/N_MICROBALL_DETS_PER_RING[ring-1];
    fDeltaOmega[ring-1][detector-1]=deltaOmega;
    fDistance[ring-1][detector-1]=distance;

    NRead++;
  }

  return NRead;
}

//____________________________________________________
double MBGeometry::GetDistance(int num_ring, int num_det) const{
	return fDistance[num_ring-1][num_det-1];
}

//____________________________________________________
double MBGeometry::GetTheta(int num_ring, int num_det) const
{
  return fThetaMean[num_ring-1][num_det-1];
}

//____________________________________________________
double MBGeometry::GetPhi(int num_ring, int num_det) const
{
  return fPhiMean[num_ring-1][num_det-1];
}

//____________________________________________________
double MBGeometry::GetThetaRandom(int num_ring, int num_det) const
{
  return fRandTheta->GetRandom(fThetaMin[num_ring-1][num_det-1], fThetaMax[num_ring-1][num_det-1]);
}

//____________________________________________________
double MBGeometry::GetPhiRandom(int num_ring, int num_det) const
{
  return gRandom->Uniform(fPhiMin[num_ring-1][num_det-1], fPhiMax[num_ring-1][num_det-1]);
}

//____________________________________________________
double MBGeometry::GetSolidAngle(int num_ring, int num_det) const
{
  return fDeltaOmega[num_ring-1][num_det-1];
}
