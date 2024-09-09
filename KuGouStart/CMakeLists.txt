cmake_minimum_required(VERSION 3.10)

project(KuGouAppStart VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools Multimedia)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SOURCE_CPP}
    ${SOURCE_H}
)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../GuessLoveWidget
    ${PROJECT_SOURCE_DIR}/../AdvertiseBoardWidget)

add_executable(${PROJECT_NAME} ${SOURCE_FILE} "Res.qrc" "KuGouApp.ui")

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools Qt6::Multimedia
    MusicTableWidget MusicItemWidget AdvertiseBoardWidget GuessLoveWidget)
