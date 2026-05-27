#ifndef TDCSPARECHANNELS_H
#define TDCSPARECHANNELS_H

#include <fstream>
#include <sstream>
#include <TTree.h>
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <vector>
#include <string>
#include <map>

class TDCSpareChannels
{
public :
  TDCSpareChannels();
  ~TDCSpareChannels();

  int InitFromMapping(TTreeReader *, const char *);      // Inits all branches for reading as defined in the mapping file of the corresponding run for detector TDCSpare
  int SetNewBranch(TTreeReader *, const char * name);    // Sets a new branch for reading
  double GetBranchValue(const char * name);              // Get a value from one of the set branches
  int SetOutputTree(TTree *);                            // Sets all the known branches in the output tree
  void FillOutputBranches();                             // Fills the fData array for the current event


private :
  int fNumChannels;
  std::string fDetName;                                  //Name of this TDCSpareChannels as in the .map file
  std::map <std::string, double *> fData;                // This is set only when FillOutputBranches() is called.
  std::map <std::string, TTreeReaderValue<double>*> fReaderData;

  int ParseMapLine(TTreeReader *, const char *);

};

#endif
