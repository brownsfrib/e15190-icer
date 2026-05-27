#ifndef NWCOSMICRAYMANAGER_H
#define NWCOSMICRAYMANAGER_H

#include <fstream>
#include <sstream>
#include <string>

class NWCosmicRayManager
{
public :
  NWCosmicRayManager(int num_bars=25);
  ~NWCosmicRayManager();

  int LoadPeakPositions(const char *);

  double GetCosmicRayCenter(int) const;           //Get the position in channel of the cosmic ray peak
  double GetCosmicRayStartingPoint(int) const;    //Get the position in channel of the cosmic ray peak - 3sigma

private :
  int fNumBars;
  double * fCenter;
  double * fSigma;

  bool fPositionLoaded;

};

#endif
