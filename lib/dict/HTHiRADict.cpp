// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libdIdictdIHTHiRADict
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
#include "HTHiRARootEvent.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_HTHiRAStripData(void *p = nullptr);
   static void *newArray_HTHiRAStripData(Long_t size, void *p);
   static void delete_HTHiRAStripData(void *p);
   static void deleteArray_HTHiRAStripData(void *p);
   static void destruct_HTHiRAStripData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HTHiRAStripData*)
   {
      ::HTHiRAStripData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HTHiRAStripData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HTHiRAStripData", ::HTHiRAStripData::Class_Version(), "HTHiRARootEvent.h", 6,
                  typeid(::HTHiRAStripData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HTHiRAStripData::Dictionary, isa_proxy, 4,
                  sizeof(::HTHiRAStripData) );
      instance.SetNew(&new_HTHiRAStripData);
      instance.SetNewArray(&newArray_HTHiRAStripData);
      instance.SetDelete(&delete_HTHiRAStripData);
      instance.SetDeleteArray(&deleteArray_HTHiRAStripData);
      instance.SetDestructor(&destruct_HTHiRAStripData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HTHiRAStripData*)
   {
      return GenerateInitInstanceLocal(static_cast<::HTHiRAStripData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HTHiRAStripData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_HTHiRACsIData(void *p = nullptr);
   static void *newArray_HTHiRACsIData(Long_t size, void *p);
   static void delete_HTHiRACsIData(void *p);
   static void deleteArray_HTHiRACsIData(void *p);
   static void destruct_HTHiRACsIData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HTHiRACsIData*)
   {
      ::HTHiRACsIData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HTHiRACsIData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HTHiRACsIData", ::HTHiRACsIData::Class_Version(), "HTHiRARootEvent.h", 21,
                  typeid(::HTHiRACsIData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HTHiRACsIData::Dictionary, isa_proxy, 4,
                  sizeof(::HTHiRACsIData) );
      instance.SetNew(&new_HTHiRACsIData);
      instance.SetNewArray(&newArray_HTHiRACsIData);
      instance.SetDelete(&delete_HTHiRACsIData);
      instance.SetDeleteArray(&deleteArray_HTHiRACsIData);
      instance.SetDestructor(&destruct_HTHiRACsIData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HTHiRACsIData*)
   {
      return GenerateInitInstanceLocal(static_cast<::HTHiRACsIData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HTHiRACsIData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_HTHiRAData(void *p = nullptr);
   static void *newArray_HTHiRAData(Long_t size, void *p);
   static void delete_HTHiRAData(void *p);
   static void deleteArray_HTHiRAData(void *p);
   static void destruct_HTHiRAData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HTHiRAData*)
   {
      ::HTHiRAData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HTHiRAData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HTHiRAData", ::HTHiRAData::Class_Version(), "HTHiRARootEvent.h", 35,
                  typeid(::HTHiRAData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HTHiRAData::Dictionary, isa_proxy, 4,
                  sizeof(::HTHiRAData) );
      instance.SetNew(&new_HTHiRAData);
      instance.SetNewArray(&newArray_HTHiRAData);
      instance.SetDelete(&delete_HTHiRAData);
      instance.SetDeleteArray(&deleteArray_HTHiRAData);
      instance.SetDestructor(&destruct_HTHiRAData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HTHiRAData*)
   {
      return GenerateInitInstanceLocal(static_cast<::HTHiRAData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HTHiRAData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_HTHiRARootEvent(void *p);
   static void deleteArray_HTHiRARootEvent(void *p);
   static void destruct_HTHiRARootEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HTHiRARootEvent*)
   {
      ::HTHiRARootEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HTHiRARootEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HTHiRARootEvent", ::HTHiRARootEvent::Class_Version(), "HTHiRARootEvent.h", 49,
                  typeid(::HTHiRARootEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HTHiRARootEvent::Dictionary, isa_proxy, 4,
                  sizeof(::HTHiRARootEvent) );
      instance.SetDelete(&delete_HTHiRARootEvent);
      instance.SetDeleteArray(&deleteArray_HTHiRARootEvent);
      instance.SetDestructor(&destruct_HTHiRARootEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HTHiRARootEvent*)
   {
      return GenerateInitInstanceLocal(static_cast<::HTHiRARootEvent*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HTHiRARootEvent*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr HTHiRAStripData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HTHiRAStripData::Class_Name()
{
   return "HTHiRAStripData";
}

//______________________________________________________________________________
const char *HTHiRAStripData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAStripData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HTHiRAStripData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAStripData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HTHiRAStripData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAStripData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HTHiRAStripData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAStripData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HTHiRACsIData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HTHiRACsIData::Class_Name()
{
   return "HTHiRACsIData";
}

//______________________________________________________________________________
const char *HTHiRACsIData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRACsIData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HTHiRACsIData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRACsIData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HTHiRACsIData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRACsIData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HTHiRACsIData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRACsIData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HTHiRAData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HTHiRAData::Class_Name()
{
   return "HTHiRAData";
}

//______________________________________________________________________________
const char *HTHiRAData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HTHiRAData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HTHiRAData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HTHiRAData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRAData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HTHiRARootEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HTHiRARootEvent::Class_Name()
{
   return "HTHiRARootEvent";
}

//______________________________________________________________________________
const char *HTHiRARootEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRARootEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HTHiRARootEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HTHiRARootEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HTHiRARootEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRARootEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HTHiRARootEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HTHiRARootEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void HTHiRAStripData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTHiRAStripData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HTHiRAStripData::Class(),this);
   } else {
      R__b.WriteClassBuffer(HTHiRAStripData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HTHiRAStripData(void *p) {
      return  p ? new(p) ::HTHiRAStripData : new ::HTHiRAStripData;
   }
   static void *newArray_HTHiRAStripData(Long_t nElements, void *p) {
      return p ? new(p) ::HTHiRAStripData[nElements] : new ::HTHiRAStripData[nElements];
   }
   // Wrapper around operator delete
   static void delete_HTHiRAStripData(void *p) {
      delete (static_cast<::HTHiRAStripData*>(p));
   }
   static void deleteArray_HTHiRAStripData(void *p) {
      delete [] (static_cast<::HTHiRAStripData*>(p));
   }
   static void destruct_HTHiRAStripData(void *p) {
      typedef ::HTHiRAStripData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HTHiRAStripData

//______________________________________________________________________________
void HTHiRACsIData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTHiRACsIData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HTHiRACsIData::Class(),this);
   } else {
      R__b.WriteClassBuffer(HTHiRACsIData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HTHiRACsIData(void *p) {
      return  p ? new(p) ::HTHiRACsIData : new ::HTHiRACsIData;
   }
   static void *newArray_HTHiRACsIData(Long_t nElements, void *p) {
      return p ? new(p) ::HTHiRACsIData[nElements] : new ::HTHiRACsIData[nElements];
   }
   // Wrapper around operator delete
   static void delete_HTHiRACsIData(void *p) {
      delete (static_cast<::HTHiRACsIData*>(p));
   }
   static void deleteArray_HTHiRACsIData(void *p) {
      delete [] (static_cast<::HTHiRACsIData*>(p));
   }
   static void destruct_HTHiRACsIData(void *p) {
      typedef ::HTHiRACsIData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HTHiRACsIData

//______________________________________________________________________________
void HTHiRAData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTHiRAData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HTHiRAData::Class(),this);
   } else {
      R__b.WriteClassBuffer(HTHiRAData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HTHiRAData(void *p) {
      return  p ? new(p) ::HTHiRAData : new ::HTHiRAData;
   }
   static void *newArray_HTHiRAData(Long_t nElements, void *p) {
      return p ? new(p) ::HTHiRAData[nElements] : new ::HTHiRAData[nElements];
   }
   // Wrapper around operator delete
   static void delete_HTHiRAData(void *p) {
      delete (static_cast<::HTHiRAData*>(p));
   }
   static void deleteArray_HTHiRAData(void *p) {
      delete [] (static_cast<::HTHiRAData*>(p));
   }
   static void destruct_HTHiRAData(void *p) {
      typedef ::HTHiRAData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HTHiRAData

//______________________________________________________________________________
void HTHiRARootEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTHiRARootEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HTHiRARootEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(HTHiRARootEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_HTHiRARootEvent(void *p) {
      delete (static_cast<::HTHiRARootEvent*>(p));
   }
   static void deleteArray_HTHiRARootEvent(void *p) {
      delete [] (static_cast<::HTHiRARootEvent*>(p));
   }
   static void destruct_HTHiRARootEvent(void *p) {
      typedef ::HTHiRARootEvent current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HTHiRARootEvent

namespace {
  void TriggerDictionaryInitialization_HTHiRADict_Impl() {
    static const char* headers[] = {
"HTHiRARootEvent.h",
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
#line 1 "HTHiRADict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$HTHiRARootEvent.h")))  HTHiRAStripData;
class __attribute__((annotate("$clingAutoload$HTHiRARootEvent.h")))  HTHiRACsIData;
class __attribute__((annotate("$clingAutoload$HTHiRARootEvent.h")))  HTHiRAData;
class __attribute__((annotate("$clingAutoload$HTHiRARootEvent.h")))  HTHiRARootEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "HTHiRADict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "HTHiRARootEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HTHiRACsIData", payloadCode, "@",
"HTHiRAData", payloadCode, "@",
"HTHiRARootEvent", payloadCode, "@",
"HTHiRAStripData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("HTHiRADict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_HTHiRADict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_HTHiRADict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_HTHiRADict() {
  TriggerDictionaryInitialization_HTHiRADict_Impl();
}
