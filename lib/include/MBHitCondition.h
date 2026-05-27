#ifndef MBHITCONDITION_H
#define MBHITCONDITION_H

/* MBHitCondition class
 * v2.0 09/20/2018 - Daniele Dell'Aquila
 * This class decides whether a raw hit (fast, slow, time) in the Microball
 * is or not a physical hit.
 * The input file, loaded with the method "LoadFastSlowCuts", must contain
 * root graphical cuts of the type "TCutG", identifying the cuts on fast/slow used to define the hit.
 * Optionally, one can also give cuts on noise, that are used to exclude noise counts.
 * Cuts should be named as follow (assuming ring XX, detector YY):
 * FSCut_RXX_YY
 * NoiseCut_RXX_YY
 *
 */

#include <TCutG.h>
#include <TFile.h>
#include <fstream>
#include <sstream>
#include <string>

#include "shared.h"

class MBHitCondition
{
public :
  MBHitCondition();
  ~MBHitCondition();

  int LoadFastSlowCuts(const char *);

  bool IsHit(int num_ring, int num_det, double fast, double slow, double time) const;

private :
  TCutG *** fFiducialCuts;
  TCutG *** fNoiseFiducialCuts;
  TFile * fFileCuts;
  bool fCutsLoaded;

};

#endif
