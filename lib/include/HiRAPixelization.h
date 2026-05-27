#ifndef HIRAPIXELIZATION_H
#define HIRAPIXELIZATION_H

#include <elist.h>
#include <HiRAPixel.h>
#include <HTHiRARootEvent.h>
#include <HiRACalibratedRootEvent.h>

#include <shared.h>

class HiRAPixelization
{
public :
  HiRAPixelization(int);
  ~HiRAPixelization();

  int Pixelization(int ntel, HiRACalibratedBuffer * pEvent, HiRACalibratedData * pCalibratedEvent);

private :
  int fNumTel;

  HiRAPixel ** fpixel;
};

#endif
