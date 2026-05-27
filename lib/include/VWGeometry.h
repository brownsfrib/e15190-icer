#ifndef VWGEOMETRY_H
#define VWGEOMETRY_H

// This class is created by Kuan to calculate the real physical position(in experiment vault coordinates, target on origin) of every hit on Neutron Wall
// 09/10/2018 - Kuan Zhu (zhuk@nscl.msu.edu)

#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <TRandom3.h>

#include <iostream>

#include <TVector3.h>

//const double bar_length = 227.237 ; // more details are in excel file "VW_positioninfo"
//const double bar_width = 9.4 ; // width of VW bar
//const double bar_thickness = 1 ; // thickness of VW bar

class VWGeometry
{
private:
	int fNumBars;
	TVector3 **fVW_center_cor;
	const double bar_length;
	const double bar_width;
	const double bar_thickness;
	const TVector3 *bar_length_direction;
	const TVector3 *bar_width_direction;
	const TVector3 *bar_thickness_direction;
	bool fFiducialPointsLoaded;

public:
	VWGeometry(int NumberOfBars);
	~VWGeometry();

	int LoadFiducialPoints(const char*);

	double GetX(int,double) const;
	double GetY(int,double) const;
	double GetZ(int,double) const;
	double GetXRan(int,double) const;
	double GetZRan(int,double) const;

	double GetTheta(int,double) const;
	double GetPhi(int,double) const;
	double GetR(int,double) const;
	double GetThetaRan(int,double) const;
	double GetPhiRan(int,double) const;
	double GetRRan(int,double) const;
};

#endif
