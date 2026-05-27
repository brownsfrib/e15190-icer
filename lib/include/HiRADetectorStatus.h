#ifndef HIRADETECTORSTATUS_H
#define HIRADETECTORSTATUS_H

#include <fstream>
#include <sstream>
#include <string>

#include "shared.h"

class HiRADetectorStatus
{
public :
  HiRADetectorStatus(int TotNumTel, int TotNumStripf, int TotNumStripb);
  ~HiRADetectorStatus();

  int LoadStipBad(const char *);

  bool IsStripfBad(int num_tel, int num_strip) const;
  bool IsStripbBad(int num_tel, int num_strip) const;

private :
  int fNumTel;
  int fNumStripf;
  int fNumStripb;

  bool **fStripfBad;
  bool **fStripbBad;

  bool fStripBadLoaded;

};

#endif
