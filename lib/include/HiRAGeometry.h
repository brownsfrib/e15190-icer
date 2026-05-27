#ifndef HIRAGEOMETRY_h
#define HIRAGEOMETRY_h

#include <fstream>
#include <sstream>
#include <string>
#include <TMath.h>

class HiRAGeometry
{
public :
  HiRAGeometry(int TotNumTel, int TotNumStripf, int TotNumStripb);
  ~HiRAGeometry();

  int LoadPixels(const char *);

  double GetThetaPixel(int num_tel, int stripf, int stripb) const;
  double GetPhiPixel(int num_tel, int stripf, int stripb) const;
  double GetThetaPixelDeg(int num_tel, int stripf, int stripb) const;
  double GetPhiPixelDeg(int num_tel, int stripf, int stripb) const;

private :
  int fNumTel;
  int fNumStripf;
  int fNumStripb;

  double fRadToDeg;

  double **fTheta;
  double **fPhi;
  double **fThetaDeg;
  double **fPhiDeg;

  bool fPixelsLoaded;
};

#endif
