#ifndef NWGEOMETRY_H
#define NWGEOMETRY_H

// This class is created by Kuan to calculate the real physical position(in experiment vault coordinates, target on origin) of every hit on Neutron Wall
// 06/05/2018 - Kuan Zhu (zhuk@nscl.msu.edu)

#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <TRandom3.h>

#include <iostream>

const double bar_length_raw = 193.6756 ; // more details are in excel file "NW_positioninfo"
const double bar_height = 7.62 ; // thickness of the prex is not included
const double bar_thickness = 6.985 ; // thickness of the prex is included
const double position_NW_direction[3] = {-149.332,-0.375,123.329} ; // NWB bar's orientation vector.this vector is subject to changes when NW is moved

class NWGeometry
{
private:
	double **fposition_NW_cor;
  int fNumBars;

public:
	NWGeometry(int NumberOfBars);
	~NWGeometry();

	int LoadFiducialPoints(const char*);

	double GetX(int,double) const;
	double GetY(int,double) const;
	double GetZ(int,double) const;
  double GetYRan(int,double) const;
	double GetR(int,double) const;
  double GetRRan(int,double) const;
	double GetTheta(int,double) const;
	double GetPhi(int,double) const;
  double GetThetaRan(int,double) const;
  double GetPhiRan(int,double) const;
};

#endif
