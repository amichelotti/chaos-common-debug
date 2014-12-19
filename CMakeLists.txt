cmake_minimum_required(VERSION 2.6)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

SET(debug_src source/debug.cpp )

IF(BUILD_FORCE_STATIC)
ADD_LIBRARY(common_debug STATIC ${debug_src})
ELSE()
ADD_LIBRARY(common_debug SHARED ${debug_src})
ENDIF()
ADD_LIBRARY(common_debug_static STATIC ${debug_src})

SET(debug_test_src source/main.cpp )
ADD_EXECUTABLE(test_debug ${debug_test_src})

TARGET_LINK_LIBRARIES(test_debug common_debug pthread)
FILE(GLOB headers "source/*.h" debug.h)
INSTALL(FILES ${headers} DESTINATION include/common/debug)
INSTALL_TARGETS(/lib  common_debug)
INSTALL_TARGETS(/bin test_debug)
