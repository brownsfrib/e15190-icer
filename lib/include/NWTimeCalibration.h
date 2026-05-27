#ifndef NWTIMECALIBRATION_H
#define NWTIMECALIBRATION_H

#include <fstream>
#include <sstream>
#include <string>

class NWTimeCalibration
{
public:
  NWTimeCalibration(int);
  ~NWTimeCalibration();

  int LoadCalibration(const char *);

  double GetTimeOffset(int num_bar) const;

private:
  int fNumBars;
  double *fTimeOffset;

  bool fCalibrationLoaded;

};

#endif
