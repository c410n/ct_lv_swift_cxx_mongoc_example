This example demonstrates:

  1. How to interact with C code from Swift
  2. How to interact with C++ code from Swift
  3. How to initialize the mongoc library
  4. How to use mongoc library to perform transactional commit
  5. How to use mongoc library to perform a non-transactional commit
  6. How to de-initialize the mongoc library
  
  Between the others the example illustrates how to:
  1. Create an object in C++ and pass it's physical address to Swift
  2. How to call a C++ function directly from Swift code using function pointers (aka the @convention(c) Swift approach)
  3. Contains instructions on how to setup the mongoc with MacOS/Xcode project
  4. How to operate with struct types between Swift and C/C++ aka assign their fields/properties

-----------------------------------

A set of detailed examples on how to use mongoc is located here:
https://github.com/mongodb/mongo-c-driver/tree/master/src/libmongoc/examples .

Additional examples on parsing and working with BSON are located here:
https://github.com/mongodb/mongo-c-driver/tree/master/src/libbson/examples .

-----------------------------------

In order to include the mongoc library into your Xcode/Swift project please follow the following steps:

1. Set Objective-C Bridging Header
2. Set the Library Search Path to /usr/local/Cellar/mongo-c-driver/1.15.1/lib
3. Set the Header Search Path to "$(inherited) /usr/local/Cellar/mongo-c-driver/1.15.1/include/libmongoc-1.0/ /usr/local/Cellar/mongo-c-driver/1.15.1/include/libbson-1.0/"
4. Add the following libraries to the build using "Linked Frameworks and Libraries" -> "+" -> "Add Other":

    libbson-1.0.0.0.0.dylib
    
    libmongoc-1.0.0.0.0.dylib

C vs Swift inter-operability: https://developer.apple.com/documentation/swift/imported_c_and_objective-c_apis

-----------------------------------

Problem:

1. SPM as per 10/16/2019 doesn't allow us to mix Swift with other languages inside the same target.

Solution:

1. Implement a separate library using C/C++ which would allow to use classes/abstractions (aka "Database API Layer")

-----------------------------------

Problem:

1. Starting in MongoDB 4.0, a MongoDB replica set is required to use transactions.

Solution:

1. Setup a docker based set of replicas to support mongo transactions on developers machine.

-----------------------------------

1. To generate an Xcode project:

swift package init --type=executable
swift package generate-xcodeproj

NOTE: The 1.51.1 being the latest version as per October 7 2019.


2. One way to install library is:

$ brew install mongo-c
Updating Homebrew...
==> Auto-updated Homebrew!
Updated 2 taps (homebrew/core and homebrew/cask).
==> New Formulae
atasm notifiers
==> Updated Formulae
aria2 broot deno fmt librsvg poppler terragrunt ask-cli cocoapods diff-pdf kubeseal nss sccache vte3 aws-cdk cromwell evince lerna pdftoipe telegraf zsh-completions
==> Downloading https://homebrew.bintray.com/bottles/mongo-c-driver-1.15.1.mojave.bottle.tar.gz
==> Downloading from <skipped>
######################################################################## 100.0%
==> Pouring mongo-c-driver-1.15.1.mojave.bottle.tar.gz
🍺  /usr/local/Cellar/mongo-c-driver/1.15.1: 172 files, 2.3MB


3. Alternate way is:

$ brew install cmake
$ curl -LO https://github.com/mongodb/mongo-c-driver/releases/download/1.15.1/mongo-c-driver-1.15.1.tar.gz
$ tar xzf mongo-c-driver-1.15.1.tar.gz
$ cd mongo-c-driver-1.15.1
$ mkdir cmake-build
$ cd cmake-build
$ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
$ make
$ sudo make install


4. The:

$ ld -lmongoc-1.0
ld: warning: -arch not specified
ld: warning: -macosx_version_min not specified, assuming 10.6
Undefined symbols for architecture x86_64:
"start", referenced from:
implicit entry/start for main executable
ld: symbol(s) not found for inferred architecture x86_64

Proves the presence of the library.
