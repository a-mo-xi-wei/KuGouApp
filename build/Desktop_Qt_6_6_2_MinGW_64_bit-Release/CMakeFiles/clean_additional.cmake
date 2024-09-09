# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "AdvertiseBoardWidget\\AdvertiseBoardWidget_autogen"
  "AdvertiseBoardWidget\\CMakeFiles\\AdvertiseBoardWidget_autogen.dir\\AutogenUsed.txt"
  "AdvertiseBoardWidget\\CMakeFiles\\AdvertiseBoardWidget_autogen.dir\\ParseCache.txt"
  "GuessLoveWidget\\CMakeFiles\\GuessLoveWidget_autogen.dir\\AutogenUsed.txt"
  "GuessLoveWidget\\CMakeFiles\\GuessLoveWidget_autogen.dir\\ParseCache.txt"
  "GuessLoveWidget\\GuessLoveWidget_autogen"
  "KuGouStart\\CMakeFiles\\KuGouAppStart_autogen.dir\\AutogenUsed.txt"
  "KuGouStart\\CMakeFiles\\KuGouAppStart_autogen.dir\\ParseCache.txt"
  "KuGouStart\\KuGouAppStart_autogen"
  "MusicItemWidget\\CMakeFiles\\MusicItemWidget_autogen.dir\\AutogenUsed.txt"
  "MusicItemWidget\\CMakeFiles\\MusicItemWidget_autogen.dir\\ParseCache.txt"
  "MusicItemWidget\\MusicItemWidget_autogen"
  "MusicTableWidget\\CMakeFiles\\MusicTableWidget_autogen.dir\\AutogenUsed.txt"
  "MusicTableWidget\\CMakeFiles\\MusicTableWidget_autogen.dir\\ParseCache.txt"
  "MusicTableWidget\\MusicTableWidget_autogen"
  )
endif()
