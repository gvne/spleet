# Spleet: A cross platform command line application to split audio files

## Build

### OSX
```bash
# Thirdparty
cd thirdparty
mkdir build && cd build
cmake -GXcode -DCMAKE_INSTALL_PREFIX=$(pwd)/../install ..
cmake --build . --config Release --target install
# Spleet (regenerate and build)
cd ../../
/Applications/JUCE/Projucer.app/Contents/MacOS/Projucer --resave spleet.jucer
open Builds/MacOSX/spleet.xcodeproj
```

### Linux
```bash
# Thirdparty
cd thirdparty
mkdir build && cd build
cmake -GXcode -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(pwd)/../install ..
make install
# Spleet (regenerate and build)
cd ../../
/Applications/JUCE/Projucer.app/Contents/MacOS/Projucer --resave spleet.jucer
cd Builds/LinuxMakefile
make
cp ../../thirdparty/install/lib/libtensorflow*.so.1 build/
```
