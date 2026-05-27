#include <HTDAQStackInfo.h>

//________________________________________________
HTDAQStackInfo::HTDAQStackInfo(const char * name, int stackID) :
fNModules(0),
fStackName(name),
fStackID(stackID)
{}

//________________________________________________
HTDAQStackInfo::~HTDAQStackInfo()
{
  Clear();
}

//________________________________________________
void HTDAQStackInfo::Clear()
{
  for(int i=0; i<fNModules; i++) {
    if(fModuleInStack[i]) {
      delete fModuleInStack[i];
    }
  }
  fModuleInStack.clear();
  fNModules=0;
}

//________________________________________________
int HTDAQStackInfo::GetNModules() const
{
  return fNModules;
}

//________________________________________________
/*
RBElectronics * HTDAQStackInfo::GetModule(int n_module) const
{
  return fModuleInStack[n_module]->GetModule();
}
*/

//________________________________________________
const char * HTDAQStackInfo::GetModuleType(int n_module) const
{
  return fModuleInStack[n_module]->GetModuleType();
}

//________________________________________________
int HTDAQStackInfo::GetModuleVSN(int n_module) const
{
  return fModuleInStack[n_module]->GetVSN();
}

//________________________________________________
int HTDAQStackInfo::GetStackID() const
{
  return fStackID;
}

//________________________________________________
const char * HTDAQStackInfo::GetStackName() const
{
  return fStackName.c_str();
}

//________________________________________________
HTModuleInfo  * HTDAQStackInfo::GetModuleInfo(int n_module) const
{
  return fModuleInStack[n_module];
}

//________________________________________________
void HTDAQStackInfo::AddModuleInfo(HTModuleInfo * new_module_info)
{
  fNModules++;
  fModuleInStack.push_back(new_module_info);
  return;
}
