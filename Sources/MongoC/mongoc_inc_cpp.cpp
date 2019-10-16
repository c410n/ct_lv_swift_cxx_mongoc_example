#include "mongoc_inc_cpp.h"

extern "C" {
    void printSomething() { // This function will be called from Swift as type: (@convention(c) () -> Void)
        puts("Printing something");
    }
}

MongocCxxClass::MongocCxxClass() {}

void MongocCxxClass::getInput(int *output) {
    *output = 92;
}

void MongocCxxClass::accessObjProperties(MongoObjData* mongoObjData) {
    puts(mongoObjData->str_data);
    if (mongoObjData->bool_data)
        std::cout << "True" << std::endl;
    mongoObjData->initObj = printSomething; // This func pointer will be called from Swift as type: (@convention(c) () -> Void)
}

//------------------------------------------------
// C++ function calls wrappers declarations follow
//------------------------------------------------

void getInputWrapper(int *output) {
    MongocCxxClass mongocCxxClass;
    mongocCxxClass.getInput(output);
}

void accessObjPropertiesWrapper(MongoObjData* mongoObjData) {
    MongocCxxClass mongocCxxClass;
    mongocCxxClass.accessObjProperties(mongoObjData);
}

void* createPersistentMongoCxxClass() {
    std::cout << "__cplusplus:" << __cplusplus << std::endl;
    return new MongocCxxClass();
}

void destroyPersistentMongoCxxClass(void* mongocCxxClass) {
    delete((MongocCxxClass*)mongocCxxClass);
}
