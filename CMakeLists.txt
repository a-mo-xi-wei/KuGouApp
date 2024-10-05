cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

#自定义标题栏模块
add_subdirectory(TitleWidget)

#广告模块
add_subdirectory(AdvertiseBoardWidget)

#猜你喜欢模块
add_subdirectory(GuessLoveWidget)

#音乐单项模块
add_subdirectory(MusicItemWidget)

#音乐表单模块
add_subdirectory(MusicTableWidget)

#遮罩模块
add_subdirectory(SMaskWidget)

#涟漪模块
add_subdirectory(RippleButton)

#音量调剂模块
add_subdirectory(VolumeToolBtn)

#滑动条模块
add_subdirectory(SliderWidget)

#开始模块
add_subdirectory(KuGouStart)
