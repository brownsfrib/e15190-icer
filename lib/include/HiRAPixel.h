#ifndef HIRAPIXEL_H
#define HIRAPIXEL_H

#include <elist.h>
#include <HiRACalibratedRootEvent.h>

#include <shared.h>

class HiRAPixel
{
 public :
  HiRAPixel(int id0);
  ~HiRAPixel();

  void LoadStrips(int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int);
  void Reset();

  int multiHit();
  int multiHitCsi();

  // Reconstructed pixels
  int fmultipixel;
  int *fnumtel;
  int *fnumstripf;
  int *fnumstripb;
  int *fnumcsi;
  double *fEnergySif;
  double *fEnergySib;
  double *fEnergyCsI;
  double *fEnergy;
  int *fstripfindex;
  int *fstripbindex;
  int *fcsiindex;
  int *fipid;
  int *fidZ;
  int *fidA;

  elist * Front;
  elist * Back;
  elist * Csi;

 private:
   int id;

   double fenergy;
   double benergy;

   int CsIhit;

   void loop(int);
   int NestDim;
   int NestArray[50];
   int arrayD[50];
   int arrayB[50];
   float deMin;
   int dstripMin;
   int FrontLow[4];
   int FrontHigh[4];
   int BackLow[4];
   int BackHigh[4];
};

#endif
