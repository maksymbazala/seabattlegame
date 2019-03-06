cmake_minimum_required(VERSION 3.12)
project(seabattlegame_xcodeproj)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cmake-build-debug)
include_directories(project.xcworkspace)
include_directories(xcuserdata)

add_executable(seabattlegame_xcodeproj
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        cmake-build-debug/seabattlegame_xcodeproj.cbp
        project.xcworkspace/xcshareddata/IDEWorkspaceChecks.plist
        project.xcworkspace/xcuserdata/maksymbazala.xcuserdatad/UserInterfaceState.xcuserstate
        project.xcworkspace/contents.xcworkspacedata
        xcuserdata/maksymbazala.xcuserdatad/xcschemes/xcschememanagement.plist
        CMakeLists.txt
        project.pbxproj)
