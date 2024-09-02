cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools Multimedia)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#message(STATUS "######## ${SOURCE_H}")

set(SOURCE_FILES
        ${SOURCE_CPP}
        ${SOURCE_H}#CMake 会自动处理包含在源文件中的头文件 , 因此这行可以不加。
        Res.qrc
        KuGouApp.ui
        style.css
)
# 设置并行编译线程数，可以根据你的CPU核心数进行调整
set(CMAKE_BUILD_PARALLEL_LEVEL 8)  # 或者设置为你机器的核心数

# 对于 Release 版本启用优化
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2")
# 对于所有构建类型启用并行编译
#add_compile_options(/MP)

set(CMAKE_BUILD_TYPE Release)  # 设置构建类型为Release
#[[
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")  # 设置MSVC编译器的并行编译选项
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -j")  # 设置GCC编译器的并行编译选项
endif()
]]
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools Qt6::Multimedia)
