#ifndef mongoc_inc_cpp_h
#define mongoc_inc_cpp_h

#include <stdio.h>
#include <iostream>

extern "C"
{
    #include "mongoc_inc.h"
    // Basically we could use mongoc from C++ here with the C++ class below f.ex. to use the power of stl/boost/e.t.c. but we will do it in mongoc_inc.c
}

class MongocCxxClass {
public:
    MongocCxxClass();
    
    void getInput(int *);
    void accessObjProperties(MongoObjData*);
};

#ifdef __cplusplus
extern "C" {
#endif
    void someLogicWrapper();
    void getInputWrapper(int*);
    void accessObjPropertiesWrapper(MongoObjData*);
    void* createPersistentMongoCxxClass();
    void destroyPersistentMongoCxxClass(void*);
#ifdef __cplusplus
}
#endif

#endif /* mongoc_inc_cpp_h */
