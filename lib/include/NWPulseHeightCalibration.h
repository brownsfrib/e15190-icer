#ifndef NWPULSEHEIGHTCALIBRATION_H
#define NWPULSEHEIGHTCALIBRATION_H

#include <string>
#include <fstream>
#include <sstream>
#include <TRandom3.h>

class NWPulseHeightCalibration
{
public :
  NWPulseHeightCalibration(int TotNumBars);
  ~NWPulseHeightCalibration();

  int LoadMatching(const char *);
  int LoadCalibration(const char *);
  int LoadPulseHeightCalibration(const char *);//KZ
  int LoadSaturationCorrection(const char *);//KZ

  double GetLeftMatched(double ch, int numbar) const;
  double GetRightMatched(double ch, int numbar) const;
  double GetLeftSaturationCorrected(double ch_left, double ch_right, double Xcm, int num_bar) const;
  double GetRightSaturationCorrected(double ch_left, double ch_right, double Xcm, int num_bar) const;
  double GetfastLeftSaturationCorrected(double fast_ch_left, double fast_ch_right, double Xcm, int num_bar) const;
  double GetfastRightSaturationCorrected(double fast_ch_left, double fast_ch_right, double Xcm, int num_bar) const;
  double GetPulseHeightCalibrated(double ch, double NW_fXcm, int numbar) const;//KZ
  double GetSaturationCorrected(double ch_left, double ch_right, double NW_fXcm, int numbar) const;//KZ

private :
  int fNumBars;
  double * fRightMatchingParameters;
  double * fLeftMatchingParameters;
  double * fPulseHeightCalibrationParameters_a;//KZ
  double * fPulseHeightCalibrationParameters_b;//KZ
  double * fPulseHeightCalibrationParameters_c;//KZ
  double * fPulseHeightCalibrationParameters_d;//KZ
  double * fPulseHeightCalibrationParameters_e;//KZ
  double * fSaturationCorrectionParameters_a;//KZ
  double * fSaturationCorrectionParameters_b;//KZ
  double * fSaturationCorrectionParameters_c;//KZ
  double * fSaturationCorrectionParameters_d;//KZ
  double * fSaturationCorrectionParameters_e;//KZ
  double * fSaturationCorrectionParameters_f;//KZ

  bool fMatchingLoaded;
  bool fCalibrationLoaded;
  bool fPositionCorrectionLoaded;
  bool fPulseHeightCalibrationLoaded;//KZ
  bool fSaturationCorrected;//KZ

};

#endif
