# SNE - Simple Network Engine

(Not) Simple Network framework for C++

## Requirements

-   Linux, MacOS: recent C++ compiler
-   Windows: Visual Studio 2015 or above
-   cmake, vcpkg

## Clone

```
git clone --recurse-submodules <sne-git-repository-url>
```

## Install on Windows

-   Install vcpkg

-   Install packages using vcpkg

    ```
    vcpkg install boost cryptopp gtest
    ```

-   Build SNE using Visual studio 2015 or above

    use `SNE_ROOT\_build\vs2015\SNE.sln`

-   or Build SNE using cmake(and Visual Studio 2017)

    ```
    echo "DLL version"
    mkdir build
    cd build
    cmake -G "Visual Studio 15 2017 Win64" -T host=x64 \
      -DCMAKE_TOOLCHAIN_FILE="<vcpkg root>\scripts\buildsystems\vcpkg.cmake" \
      -DVCPKG_TARGET_TRIPLET:STRING=x64-windows ..
    cmake --build . --config Release
    ```

    ```
    echo "Static version"
    mkdir build
    cd build
    cmake -G "Visual Studio 15 2017 Win64" -T host=x64 \
      -DSNE_MSVC_STATIC_RUNTIME=ON \
      -DCMAKE_TOOLCHAIN_FILE="<vcpkg root>\scripts\buildsystems\vcpkg.cmake" \
      -DVCPKG_TARGET_TRIPLET:STRING=x64-windows-static ..
    cmake --build . --config Release
    ```

## Install on macOS

-   Install vcpkg

-   Install packages using vcpkg

    ```
    vcpkg install cryptopp gtest
    ```

-   Install Boost library using brew

    ```
    brew install boost
    ```

-   or using vcpkg with patch

    see https://stackoverflow.com/a/53480556

-   or from source codes

    Download tar-ball from http://boost.org/

    ```
    tar xf boost_xx.tar.gz
    cd boost_xx
    ./bootstrap.sh
    sudo ./b2 toolset=gcc cxxflags="-std=c++14" linkflags="-std=c++14" -j 8 install
    ```

-   SNE

    ```
    mkdir build && cd build
    cmake -DSNE_BUILD_EXAMPLES=1 -DSNE_BUILD_TESTS=1 -DBOOST_ROOT=/usr \
      -DCMAKE_TOOLCHAIN_FILE="<vcpkg root>/scripts/buildsystems/vcpkg.cmake"..
    make && make test && make install
    ```

## Install on Linux

-   Install vcpkg

-   Install packages using vcpkg

    ```
    vcpkg install boost cryptopp gtest
    ```

-   SNE

    ```
    mkdir build && cd build
    cmake -DSNE_BUILD_EXAMPLES=1 -DSNE_BUILD_TESTS=1 -DBOOST_ROOT=/usr \
      -DCMAKE_TOOLCHAIN_FILE="<vcpkg root>/scripts/buildsystems/vcpkg.cmake"..
    make && make test && make install
    ```
