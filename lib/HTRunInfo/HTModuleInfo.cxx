#include <HTModuleInfo.h>

//________________________________________________
HTModuleInfo::HTModuleInfo() :
fNumCh(0),
fModuleVSN(0),
//fModule(0),
fModuleMapping(0),
fModulePedestal(0)
{}

//________________________________________________
HTModuleInfo::~HTModuleInfo()
{
  //delete fModule;
  for (int i=0; i<fNumCh; i++) {
    if(fModuleMapping) {
      delete fModuleMapping;
    }
    if(fModulePedestal){
      delete fModulePedestal;
    }
  }
}

//________________________________________________
/*
void HTModuleInfo::SetModule(RBElectronics * newModule)
{
  fModule=newModule;
}
*/

//________________________________________________
void HTModuleInfo::SetNumCh(int num_ch)
{
  fNumCh=num_ch;
}

//________________________________________________
void HTModuleInfo::SetVSN(int new_VSN)
{
  fModuleVSN=new_VSN;
}

//________________________________________________
void HTModuleInfo::SetModuleType(std::string modType)
{
  fModuleType.assign(modType);
}

//________________________________________________
void HTModuleInfo::SetModuleType(const char * modType)
{
  fModuleType.assign(modType);
}

//________________________________________________
void HTModuleInfo::SetModuleName(std::string modName)
{
  fModuleName.assign(modName);
}

//________________________________________________
void HTModuleInfo::SetModuleName(const char * modName)
{
  fModuleName.assign(modName);
}

//________________________________________________
void HTModuleInfo::SetChannelMapping(int ch, std::string newChMapping)
{
  fModuleMapping[ch].assign(newChMapping);
}

//________________________________________________
void HTModuleInfo::SetChannelMapping(int ch, const char * newChMapping)
{
  fModuleMapping[ch].assign(newChMapping);
}

//________________________________________________
void HTModuleInfo::SetChannelPedestal(int ch, double newPedestal)
{
  fModulePedestal[ch]=newPedestal;
}

//________________________________________________
/*
RBElectronics * HTModuleInfo::GetModule() const
{
  return fModule;
}
*/

//________________________________________________
int HTModuleInfo::GetVSN() const
{
  return fModuleVSN;
}

//________________________________________________
int HTModuleInfo::GetNumCh() const
{
  return fNumCh;
}

//________________________________________________
const char * HTModuleInfo::GetModuleType() const
{
  return fModuleType.c_str();
}

//________________________________________________
const char * HTModuleInfo::GetModuleName() const
{
  return fModuleName.c_str();
}

//________________________________________________
const char * HTModuleInfo::GetModuleChMapping(int ch) const
{
  return fModuleMapping[ch].c_str();
}

//________________________________________________
double HTModuleInfo::GetModuleChPedestal(int ch) const
{
  return fModulePedestal[ch];
}
