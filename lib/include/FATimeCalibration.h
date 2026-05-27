#ifndef FATIMECALIBRATION_H
#define FATIMECALIBRATION_H

#include <fstream>
#include <sstream>
#include <string>
#include <TF1.h>

class FATimeCalibration
{
public:
  FATimeCalibration(int);
  ~FATimeCalibration();

  int LoadCalibration(const char *);
  int LoadPulseHeightCorrection(const char *);

  double GetTimeOffset(int num_det) const;
  double GetTimePulseHeightCorrection(int num_det, double pulse_height) const;

private:
  int fNumDetectors;
  double *fTimeOffset;
  TF1 ** fPulseHeightCorrectionFunc;

  bool fCalibrationLoaded;
  bool fPulseHeightCorrectionLoaded;

};

#endif
