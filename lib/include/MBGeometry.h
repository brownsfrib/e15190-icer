#ifndef MBGEOMETRY_H
#define MBGEOMETRY_H

#include <fstream>
#include <sstream>
#include <string>
#include <TRandom3.h>
#include <TF1.h>
#include <TMath.h>

#include "shared.h"

class MBGeometry
{
public :
  MBGeometry();
  ~MBGeometry();

  int LoadGeometry(const char *);
  double GetDistance(int num_ring, int num_det) const;
  double GetTheta(int num_ring, int num_det) const;
  double GetPhi(int num_ring, int num_det) const;
  double GetThetaRandom(int num_ring, int num_det) const;
  double GetPhiRandom(int num_ring, int num_det) const;
  double GetSolidAngle(int num_ring, int num_det) const;

private :
  double ** fThetaMean;
  double ** fThetaMax;
  double ** fThetaMin;
  double ** fPhiMean;
  double ** fPhiMin;
  double ** fPhiMax;
  double ** fDeltaOmega;
  double ** fDistance;

  TF1 * fRandTheta;

  double fDegToRad;
  double fRadToDeg;

};

#endif
