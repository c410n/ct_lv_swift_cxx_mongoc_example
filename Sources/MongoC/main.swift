var tmp:CInt = 0


// Changing the value of tmp from C++
getInputWrapper(&tmp)
print("Hello, world: \(Int(tmp))") // Int() conversion not required in this case, just an example


// Declaring a POC object
var mongoObjData1: MongoObjData = MongoObjData()
mongoObjData1.bool_data = true
let str_data: String = "My wonderful String"
// Assiging a Swift string data to the field of the Swift structure
mongoObjData1.str_data = UnsafeMutablePointer(mutating: str_data)
// Printing the values assigned to the mongoObjData1 fields from C++ code
accessObjPropertiesWrapper(&mongoObjData1)


// Getting a function pointer from the mongoObjData1 object (the pointer was previously assigned with accessObjPropertiesWrapper function call at line 16)
var initFunc : (@convention(c) () -> Void) = mongoObjData1.initObj // Setting a function pointer from C++ and then calling it from Swift
// Calling a C++ C-convention declared function pointer
initFunc()

// Creating a C++ class and getting a valid pointer to the allocated object in memory
let objPtr = createPersistentMongoCxxClass()
// Printing the address of the object in memory
print(objPtr!)
// Destructing C++ class using a pointer from inside C++ code
destroyPersistentMongoCxxClass(objPtr)

// Demonstrating basic abilities of the MongoC software
var mongoObjData2: MongoObjData = MongoObjData()
if createDatabaseAndSession() == false {
    writeDataToCollection()
    writeDataToCollectionNoTransaction()
    closeDatabase()
}
