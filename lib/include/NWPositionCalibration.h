#ifndef NWPOSITIONCALIBRATION_H
#define NWPOSITIONCALIBRATION_H

#include <fstream>
#include <sstream>
#include <string>

class NWPositionCalibration
{
public :
  NWPositionCalibration(int num_bars=25);
  ~NWPositionCalibration();

  int LoadCalibration(const char *);

  double GetPosition(int, double, double) const;  //Get the position in centimeters calculated from the two PMT times

private :
  int fNumBars;
  double * fOffset;
  double * fSpeedLightBar;

  bool fCalibrationLoaded;

};

#endif
