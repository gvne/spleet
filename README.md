# Spleet: A cross platform command line application to split audio files

## Build
### Thirdparty

#### OSX
```bash
cd thirdparty
mkdir build && cd build
cmake -GXcode -DCMAKE_INSTALL_PREFIX=$(pwd)/../install ..
cmake --build . --config Release --target install
```
