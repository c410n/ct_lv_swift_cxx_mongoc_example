#ifndef mongoc_inc_h
#define mongoc_inc_h

/*
 1. Set Objective-C Bridging Header
 2. Set the Library Search Path to /usr/local/Cellar/mongo-c-driver/1.15.1/lib
 3. Set the Header Search Path to "$(inherited) /usr/local/Cellar/mongo-c-driver/1.15.1/include/libmongoc-1.0/ /usr/local/Cellar/mongo-c-driver/1.15.1/include/libbson-1.0/"
 4. Add the following libraries to the build using "Linked Frameworks and Libraries" -> "+" -> "Add Other":
    libbson-1.0.0.0.0.dylib
    libmongoc-1.0.0.0.0.dylib
 
 C vs Swift inter-operability: https://developer.apple.com/documentation/swift/imported_c_and_objective-c_apis
*/

#include <stdio.h>
#include <stdbool.h>
#include <mongoc/mongoc.h>
#include <bson.h>

typedef struct {
    char* str_data;
    int bool_data; // C99 doesn't have bool being defined
    void (*initObj)(void);
} MongoObjData;

bool createDatabaseAndSession();
bool writeDataToCollection();
bool writeDataToCollectionNoTransaction();
bool closeDatabase();

#endif /* mongoc_inc_h */
