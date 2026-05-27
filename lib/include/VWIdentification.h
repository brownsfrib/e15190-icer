#ifndef VWIDENTIFICATION_H
#define VWIDENTIFICATION_H

#include <TCutG.h>
#include <TFile.h>

/* VWIdentification class
 * 2018/08/31
 * Created by Daniele Dell'Aquila (dellaqui@nscl.msu.edu)
 * The class can handle VW charged particle identifications up to Z=5 and A=6
 * please change dVWIdentificationZMAX and dVWIdentificationAMAX, respectively, to change these values
 *
 *
 */

#define dVWIdentificationZMAX 5
#define dVWIdentificationAMAX 6

class VWIdentification
{
public :
  VWIdentification();
  ~VWIdentification();

  int LoadIdentificationCuts(const char *);

  bool IsChargedParticle(double DE, double TOF) const;
  int GetZ(double DE, double TOF) const;
  int GetA(double DE, double TOF) const;

private :
  TCutG *** fIdentificationCuts;  //fIdentificationCuts[Z][A] is a TCutG*
  TFile * fFileCuts;
  bool fCutsLoaded;
};

#endif
