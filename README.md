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
~/JUCE/Projucer --resave spleet.jucer
cd Builds/LinuxMakefile
make
# Copy libs and models
cp ../../thirdparty/install/lib/libtensorflow*.so.1 build/
rm -rf build/models
cp -r ../../thirdparty/install/models/default build/models
```

### Windows (VS2019)
```bash
cd thirdparty
mkdir build && cd build
cmake -GXcode -DCMAKE_INSTALL_PREFIX=$(pwd)/../install ..
cmake --build . --config Release --target INSTALL
# Spleet (regenerate and build)
cd ../../
/c/JUCE/Projucer.exe --resave spleet.jucer
cd Builds/VisualStudio2019/
start spleet.sln
# TODO: Build in Release mode
# Copy dll and models
cp ../../thirdparty/install/lib/tensorflow.dll x64/Release/ConsoleApp
rm -rf x64/Release/ConsoleApp/models
cp -r ../../thirdparty/install/models/default x64/Release/ConsoleApp/models
```
