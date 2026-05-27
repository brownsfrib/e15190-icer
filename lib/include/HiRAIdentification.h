#ifndef HIRAIDENTIFICATION_H
#define HIRAIDENTIFICATION_H

#include <HiRACalibratedRootEvent.h>
#include <ZApar.h>

class HiRAIdentification
{
public :
  HiRAIdentification();
  ~HiRAIdentification();

  int LoadIdentification(const char *);

  int PerformIdentification(HiRACalibratedData *);
  int GetIdentifiedZ(double ERes, double DE);
  int GetIdentifiedA(double ERes, double DE);

private :
  ZApar ** fpar;
  int fNumLines;

  bool fIdentificationLoaded;

};

#endif
