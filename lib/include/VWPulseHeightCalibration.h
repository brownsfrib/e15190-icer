#ifndef VWPULSEHEIGHTCALIBRATION_H
#define VWPULSEHEIGHTCALIBRATION_H

#include <fstream>
#include <sstream>
#include <string>

class VWPulseHeightCalibration
{
public :
  VWPulseHeightCalibration(int num_bars);
  ~VWPulseHeightCalibration();

  int LoadGainMatching(const char *);

  double GetGeoMeanMatched(double ch, int num_bar) const;

private :
  int fNumBars;
  double * fGeoMeanScalingFactors;

  bool fGainMatched;

};

#endif
