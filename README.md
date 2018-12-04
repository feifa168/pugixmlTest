## 简介
>pugixmlTest用于测试pugixml库，创建xml，读取xml，支持从文件，内存，iostream，支持xpath查找。

## 使用说明
* 添加头文件
> CMakeLists.txt中增加 set(INC_DIR ./ include/pugixml)，包含pugixml.hpp和pugiconfig.hpp
* 添加动态库
> CMakeLists.txt中增加动态库查找目录 set(LINK_DIR ./ libs/${CMAKE_BUILD_TYPE}) <br>
> CMakeLists.txt中设置加载的动态库文件 link_libraries(libpugixml)
* 调用方法
> 具体方法参见代码部分。


## CMakeListx.txt
> 详情参见CMakeLists.txt源文件
```
cmake_minimum_required(VERSION 3.12)
project(pugixmlTest)

#ADD_DEFINITIONS(-DPUGIXML_HEADER_ONLY)
set(CMAKE_CXX_STANDARD 14)

set(INC_DIR ./ include/pugixml)
set(LINK_DIR ./ libs/${CMAKE_BUILD_TYPE})
set(SOURCE_FILES main.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../libs/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../libs/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/${CMAKE_BUILD_TYPE})

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(libpugixml)

add_executable(pugixmlTest ${SOURCE_FILES})
```