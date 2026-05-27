// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libdIdictdIHTMicroballDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "HTMicroballRootEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_HTMicroballData(void *p = nullptr);
   static void *newArray_HTMicroballData(Long_t size, void *p);
   static void delete_HTMicroballData(void *p);
   static void deleteArray_HTMicroballData(void *p);
   static void destruct_HTMicroballData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HTMicroballData*)
   {
      ::HTMicroballData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HTMicroballData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HTMicroballData", ::HTMicroballData::Class_Version(), "HTMicroballRootEvent.h", 8,
                  typeid(::HTMicroballData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HTMicroballData::Dictionary, isa_proxy, 4,
                  sizeof(::HTMicroballData) );
      instance.SetNew(&new_HTMicroballData);
      instance.SetNewArray(&newArray_HTMicroballData);
      instance.SetDelete(&delete_HTMicroballData);
      instance.SetDeleteArray(&deleteArray_HTMicroballData);
      instance.SetDestructor(&destruct_HTMicroballData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HTMicroballData*)
   {
      return GenerateInitInstanceLocal(static_cast<::HTMicroballData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HTMicroballData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr HTMicroballData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HTMicroballData::Class_Name()
{
   return "HTMicroballData";
}

//______________________________________________________________________________
const char *HTMicroballData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTMicroballData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HTMicroballData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTMicroballData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HTMicroballData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTMicroballData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HTMicroballData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTMicroballData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void HTMicroballData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTMicroballData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HTMicroballData::Class(),this);
   } else {
      R__b.WriteClassBuffer(HTMicroballData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HTMicroballData(void *p) {
      return  p ? new(p) ::HTMicroballData : new ::HTMicroballData;
   }
   static void *newArray_HTMicroballData(Long_t nElements, void *p) {
      return p ? new(p) ::HTMicroballData[nElements] : new ::HTMicroballData[nElements];
   }
   // Wrapper around operator delete
   static void delete_HTMicroballData(void *p) {
      delete (static_cast<::HTMicroballData*>(p));
   }
   static void deleteArray_HTMicroballData(void *p) {
      delete [] (static_cast<::HTMicroballData*>(p));
   }
   static void destruct_HTMicroballData(void *p) {
      typedef ::HTMicroballData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HTMicroballData

namespace {
  void TriggerDictionaryInitialization_HTMicroballDict_Impl() {
    static const char* headers[] = {
"HTMicroballRootEvent.h",
nullptr
    };
    static const char* includePaths[] = {
"lib",
"lib/include",
"lib/HTRunInfo",
"/opt/install/root6/include/",
"/mnt/home/brow1247/Koonin/e15190/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "HTMicroballDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$HTMicroballRootEvent.h")))  HTMicroballData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HTMicroballDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HTMicroballRootEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HTMicroballData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HTMicroballDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HTMicroballDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HTMicroballDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HTMicroballDict() {
  TriggerDictionaryInitialization_HTMicroballDict_Impl();
}
