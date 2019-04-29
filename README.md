[![Build Status](https://travis-ci.org/akrava/web-framework.svg?branch=master)](https://travis-ci.org/akrava/web-framework)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/16090.svg)](https://scan.coverity.com/projects/akrava-web-framework)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://akrava.github.io/web-framework)
## How to use
### 1. Pre-install
Install [mustache](https://github.com/no1msd/mstch) and [nlohmann_json](https://github.com/nlohmann/json) _(it also may require boost or other libraries)_
### 2. Install
```bash
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make
 $ make install
```
### 3. Using
```cmake
find_package(akrava_web-server)
find_package(mstch)
find_package(nlohmann_json)
target_link_libraries(${PROJECT_NAME} akrava::akrava_web-server nlohmann_json::nlohmann_json mstch::mstch)
```
## Examples 
See [here](examples/)
## Documentation
see documentation [here](docs/refman.pdf)
