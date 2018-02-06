# SHared Object Module Loader for IoT.JS (SHOML.JS)


# How to test?

Clone the repository next to the IoT.js directory.

1) The following directory structure should be present now:
```sh
$ ls -1
iotjs
shoml
```

2) If the directory structure is ok, then build the iotjs and the module:
```sh
$ cd iotjs && tools/build.py --external-modules=../shoml --cmake-param=-DENABLE_MODULE_SHOML=ON --clean
```
This will also create a `libiotjs_dynmodule.so` in the IoT.js `lib` build directory.


3) After the build run the following example js file which uses the SHOML module.
For example:
``` sh
$ ./build/x86_64-linux/debug/bin/iotjs ../shoml/lib/test_so_load.js
```
