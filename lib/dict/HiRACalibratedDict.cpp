// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libdIdictdIHiRACalibratedDict
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
#include "HiRACalibratedRootEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_HiRACalibratedData(void *p = nullptr);
   static void *newArray_HiRACalibratedData(Long_t size, void *p);
   static void delete_HiRACalibratedData(void *p);
   static void deleteArray_HiRACalibratedData(void *p);
   static void destruct_HiRACalibratedData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HiRACalibratedData*)
   {
      ::HiRACalibratedData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HiRACalibratedData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HiRACalibratedData", ::HiRACalibratedData::Class_Version(), "HiRACalibratedRootEvent.h", 8,
                  typeid(::HiRACalibratedData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HiRACalibratedData::Dictionary, isa_proxy, 4,
                  sizeof(::HiRACalibratedData) );
      instance.SetNew(&new_HiRACalibratedData);
      instance.SetNewArray(&newArray_HiRACalibratedData);
      instance.SetDelete(&delete_HiRACalibratedData);
      instance.SetDeleteArray(&deleteArray_HiRACalibratedData);
      instance.SetDestructor(&destruct_HiRACalibratedData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HiRACalibratedData*)
   {
      return GenerateInitInstanceLocal(static_cast<::HiRACalibratedData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HiRACalibratedData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_HiRACalibratedBuffer(void *p = nullptr);
   static void *newArray_HiRACalibratedBuffer(Long_t size, void *p);
   static void delete_HiRACalibratedBuffer(void *p);
   static void deleteArray_HiRACalibratedBuffer(void *p);
   static void destruct_HiRACalibratedBuffer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HiRACalibratedBuffer*)
   {
      ::HiRACalibratedBuffer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HiRACalibratedBuffer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HiRACalibratedBuffer", ::HiRACalibratedBuffer::Class_Version(), "HiRACalibratedRootEvent.h", 53,
                  typeid(::HiRACalibratedBuffer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HiRACalibratedBuffer::Dictionary, isa_proxy, 4,
                  sizeof(::HiRACalibratedBuffer) );
      instance.SetNew(&new_HiRACalibratedBuffer);
      instance.SetNewArray(&newArray_HiRACalibratedBuffer);
      instance.SetDelete(&delete_HiRACalibratedBuffer);
      instance.SetDeleteArray(&deleteArray_HiRACalibratedBuffer);
      instance.SetDestructor(&destruct_HiRACalibratedBuffer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HiRACalibratedBuffer*)
   {
      return GenerateInitInstanceLocal(static_cast<::HiRACalibratedBuffer*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HiRACalibratedBuffer*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr HiRACalibratedData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HiRACalibratedData::Class_Name()
{
   return "HiRACalibratedData";
}

//______________________________________________________________________________
const char *HiRACalibratedData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HiRACalibratedData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HiRACalibratedData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HiRACalibratedData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HiRACalibratedBuffer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HiRACalibratedBuffer::Class_Name()
{
   return "HiRACalibratedBuffer";
}

//______________________________________________________________________________
const char *HiRACalibratedBuffer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedBuffer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HiRACalibratedBuffer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedBuffer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HiRACalibratedBuffer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedBuffer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HiRACalibratedBuffer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HiRACalibratedBuffer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void HiRACalibratedData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HiRACalibratedData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HiRACalibratedData::Class(),this);
   } else {
      R__b.WriteClassBuffer(HiRACalibratedData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HiRACalibratedData(void *p) {
      return  p ? new(p) ::HiRACalibratedData : new ::HiRACalibratedData;
   }
   static void *newArray_HiRACalibratedData(Long_t nElements, void *p) {
      return p ? new(p) ::HiRACalibratedData[nElements] : new ::HiRACalibratedData[nElements];
   }
   // Wrapper around operator delete
   static void delete_HiRACalibratedData(void *p) {
      delete (static_cast<::HiRACalibratedData*>(p));
   }
   static void deleteArray_HiRACalibratedData(void *p) {
      delete [] (static_cast<::HiRACalibratedData*>(p));
   }
   static void destruct_HiRACalibratedData(void *p) {
      typedef ::HiRACalibratedData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HiRACalibratedData

//______________________________________________________________________________
void HiRACalibratedBuffer::Streamer(TBuffer &R__b)
{
   // Stream an object of class HiRACalibratedBuffer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HiRACalibratedBuffer::Class(),this);
   } else {
      R__b.WriteClassBuffer(HiRACalibratedBuffer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HiRACalibratedBuffer(void *p) {
      return  p ? new(p) ::HiRACalibratedBuffer : new ::HiRACalibratedBuffer;
   }
   static void *newArray_HiRACalibratedBuffer(Long_t nElements, void *p) {
      return p ? new(p) ::HiRACalibratedBuffer[nElements] : new ::HiRACalibratedBuffer[nElements];
   }
   // Wrapper around operator delete
   static void delete_HiRACalibratedBuffer(void *p) {
      delete (static_cast<::HiRACalibratedBuffer*>(p));
   }
   static void deleteArray_HiRACalibratedBuffer(void *p) {
      delete [] (static_cast<::HiRACalibratedBuffer*>(p));
   }
   static void destruct_HiRACalibratedBuffer(void *p) {
      typedef ::HiRACalibratedBuffer current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HiRACalibratedBuffer

namespace {
  void TriggerDictionaryInitialization_HiRACalibratedDict_Impl() {
    static const char* headers[] = {
"HiRACalibratedRootEvent.h",
nullptr
    };
    static const char* includePaths[] = {
"lib",
"lib/include",
"/opt/install/root6/include/",
"/mnt/home/brow1247/Koonin/e15190/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "HiRACalibratedDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$HiRACalibratedRootEvent.h")))  HiRACalibratedData;
class __attribute__((annotate("$clingAutoload$HiRACalibratedRootEvent.h")))  HiRACalibratedBuffer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HiRACalibratedDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HiRACalibratedRootEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HiRACalibratedBuffer", payloadCode, "@",
"HiRACalibratedData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HiRACalibratedDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HiRACalibratedDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HiRACalibratedDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HiRACalibratedDict() {
  TriggerDictionaryInitialization_HiRACalibratedDict_Impl();
}
