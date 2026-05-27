#include <TDCSpareChannels.h>

//____________________________________________________
TDCSpareChannels::TDCSpareChannels() :
fNumChannels(0)
{}

//____________________________________________________
TDCSpareChannels::~TDCSpareChannels()
{
  for(std::map<std::string, double *>::iterator TheChannel = fData.begin(); TheChannel != fData.end(); TheChannel++) {
    delete (*TheChannel).second;
  }
  for(std::map<std::string, TTreeReaderValue<double>*>::iterator TheChannel = fReaderData.begin(); TheChannel != fReaderData.end(); TheChannel++) {
    delete (*TheChannel).second;
  }
}

//____________________________________________________
int TDCSpareChannels::InitFromMapping(TTreeReader * theReader, const char * file_name)
{
  std::ifstream FileIn(file_name);

  if(!FileIn.is_open()) {
    return -1;
  }

  int NChannelsMapped=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);
    std::string LineReadCommentLess(LineRead.substr(0,LineRead.find("*")));

    if(LineReadCommentLess.empty()) continue;

    if(LineReadCommentLess.find_first_not_of(' ') == std::string::npos) continue;

    if(LineReadCommentLess.find("define ")!=std::string::npos) {
      if(LineReadCommentLess.find("TDCSpare")!=std::string::npos) {
        fDetName.assign(LineReadCommentLess.substr(LineReadCommentLess.find("\"")+1,LineReadCommentLess.find_last_of("\"")-(LineReadCommentLess.find("\"")+1)));
      }
    }
    if(LineReadCommentLess.find("map ")!=std::string::npos) {
      if(LineReadCommentLess.find(fDetName)!=std::string::npos) {
        NChannelsMapped+=ParseMapLine(theReader, LineReadCommentLess.c_str());
      }
    }
  }
  FileIn.close();

  return NChannelsMapped;
}

int TDCSpareChannels::ParseMapLine(TTreeReader * theReader, const char * line_to_parse)
{
  std::string LineReadCommentLess(line_to_parse);
  std::istringstream LineStream(LineReadCommentLess.substr(LineReadCommentLess.find("map ")+4));
  std::string DetectorName;
  std::string ChName;
  std::string DetectorToSet;
  std::string ModuleName;
  std::string ModuleChannelString;
  LineStream>>DetectorName>>ChName>>DetectorToSet>>ModuleName>>ModuleChannelString;
  if(DetectorName.compare(fDetName)!=0) return 0;

  DetectorToSet.assign(DetectorToSet.substr(DetectorToSet.find("detector=\"")+10,DetectorToSet.find_last_of("\"")-(DetectorToSet.find("detector=\"")+10)));

  SetNewBranch(theReader, Form("%s.%s", fDetName.c_str(), DetectorToSet.c_str()));

  return 1;
}

//____________________________________________________
int TDCSpareChannels::SetNewBranch(TTreeReader * theReader, const char * name)
{
  if(fReaderData.find(name)==fReaderData.end()) {
    //this entry was not added before
    TTreeReaderValue<double> * newChannel = new TTreeReaderValue<double>(*theReader, name);
    fReaderData[name]=newChannel;
    double * newChannelData = new double;
    fData[name]=newChannelData;
    fNumChannels++;
    return 1;
  }
  return 0;
}

//____________________________________________________
double TDCSpareChannels::GetBranchValue(const char * name)
{
  return *(fData[name]);
}

//____________________________________________________
int TDCSpareChannels::SetOutputTree(TTree * theTree)
{
  int NumBranches=0;
  for(std::map<std::string, double *>::iterator TheChannel = fData.begin(); TheChannel != fData.end(); TheChannel++) {
    theTree->Branch((*TheChannel).first.c_str(), (*TheChannel).second, Form("%s/D", (*TheChannel).first.c_str()));
    NumBranches++;
  }
  return NumBranches;
}

//____________________________________________________
void TDCSpareChannels::FillOutputBranches()
{
  for(std::map<std::string, double *>::iterator TheChannel = fData.begin(); TheChannel != fData.end(); TheChannel++) {
    *(*TheChannel).second=*(fReaderData[(*TheChannel).first]->Get());
  }
}


