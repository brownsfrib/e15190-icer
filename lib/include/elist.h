#ifndef ELIST_H
#define ELIST_H

#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

struct order
{
  double energy;
  double energyR;  // high gain channels
  double energyRlow;  //low channels
  double energylow;
  double energyMax;
  int strip;
  int overflow;
  int neighbours;
  double time;
  int detectorindex;

};

int const nnn=60;

/**
 * !\brief Energy ordered list
 *
 * This class creates an energy ordered list of the strips
 * read out from a strip detector, keeping track of the strip
 * numbers that fired.
 */

class elist
{
 public:
  elist();
  ~elist();
  int Nstore; //number stored in list
  order Order[nnn];
  int mult;
  void Add(int,int,int,double,double);
  void Add(int,int,int,double,double,double);
  void Add(int,int,double,double,double,double);
  void Add(int,int, double, int, int);
  int  Reduce(char*);
  void reset();
  void Neighbours(string,double,double);
};

#endif
