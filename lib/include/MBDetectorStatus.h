#ifndef MBDETECTORSTATUS_H
#define MBDETECTORSTATUS_H

#include <fstream>
#include <sstream>
#include <string>

#include "shared.h"

class MBDetectorStatus
{
public :
  MBDetectorStatus();
  ~MBDetectorStatus();

  int LoadBadDetectors(const char *);

  bool IsBad(int num_ring, int num_det) const;

private :
  bool ** fBadDetectors;

};

#endif
