/* The header with help of which C makes its functions being accessible to Swift */

#ifndef ctmongoc_bridging_header_h
#define ctmongoc_bridging_header_h
#include <stdbool.h> // C99 doesn't have 'bool' being defined

/*
 Notice that this is a twin declaration of the same structure from mongoc_inc.h,
 but here for inter-operability purposes to make it being visible to Swift code
*/
typedef struct {
    char* str_data;
    bool bool_data;
    void (*initObj)(void);
} MongoObjData;

// C++ calls wrappers made visible to Swift
void someLogicWrapper();
void getInputWrapper(int*);
void accessObjPropertiesWrapper(MongoObjData*);
void* createPersistentMongoCxxClass();
void destroyPersistentMongoCxxClass(void*);


bool createDatabaseAndSession();
bool writeDataToCollection();
bool writeDataToCollectionNoTransaction();
bool closeDatabase();

#endif /* ctmongoc_bridging_header_h */
