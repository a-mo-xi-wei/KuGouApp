cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

#广告模块
add_subdirectory(AdvertiseBoardWidget)

#猜你喜欢模块
add_subdirectory(GuessLoveWidget)

#音乐单项模块
add_subdirectory(MusicItemWidget)

#音乐表单模块
add_subdirectory(MusicTableWidget)

#开始模块
add_subdirectory(KuGouStart)
