cmake_minimum_required(VERSION 2.8)
project(common_debug)

SET(debug_src core/debug.cpp )
SET(debug_test_src test/main.cpp )
ADD_DEFINITIONS(-fPIC)
IF (WIN32)
INCLUDE_DIRECTORIES(../.. ${ALL_WINDOWS_INCLUDE_DIR})
ELSE()
IF(FrameworkLib)
find_package(chaos)
SET(libs chaos_common ${FrameworkLib} dl)
ELSE()
SET(libs pthread dl)
ENDIF(FrameworkLib)
INCLUDE_DIRECTORIES(../..)
ENDIF(WIN32)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${debug_src})
IF(COMMON_TEST)
 ADD_EXECUTABLE(test_debug ${debug_test_src})

 IF (WIN32)
 TARGET_LINK_LIBRARIES(test_debug ${PROJECT_NAME} ${ALL_WINDOWS_LIBPATH})
 ELSE()
 TARGET_LINK_LIBRARIES(test_debug ${PROJECT_NAME} ${libs})
 ENDIF(WIN32)
INSTALL(TARGETS test_debug
        DESTINATION "bin/debug"
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)

ENDIF()

FILE(GLOB headers "core/*.h")
INSTALL(FILES ${headers} DESTINATION include/common/debug/core)
INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
