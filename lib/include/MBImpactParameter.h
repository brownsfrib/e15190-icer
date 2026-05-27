#ifndef MBIMPACTPARAMETER_H
#define MBIMPACTPARAMETER_H

#include <fstream>
#include <sstream>
#include <string>

#include "shared.h"

class MBImpactParameter
{
public :
  MBImpactParameter();
  ~MBImpactParameter();

  int LoadImpactParameter(const char *);

  double GetImpactParameter(int) const;
  double GetImpactParameterError(int) const;
  double Getbhat(int) const;
  double GetbhatError(int) const;

private :
  double *fImpactParameters;
  double *fImpactParameterErrors;
  double *fbhat;
  double *fbhatErrors;

  bool fImpactParameterLoaded;
};

#endif
