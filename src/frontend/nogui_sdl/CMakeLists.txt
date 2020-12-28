project(nogui_sdl)

SET(SOURCES_NOGUI_SDL
    main.cpp
    main_shaders.h
    Input.cpp
    LAN_PCap.cpp
    LAN_Socket.cpp
    OSD.cpp
    OSD_shaders.h
    font.h
    Platform.cpp
    PlatformConfig.cpp

    ../Util_ROM.cpp
    ../Util_Video.cpp
    ../Util_Audio.cpp
    ../FrontendUtil.h
    ../mic_blow.h
)

if (WIN32)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -i <SOURCE> -o <OBJECT>")
endif()

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Iconv REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SLIRP REQUIRED slirp)

if (WIN32 AND (CMAKE_BUILD_TYPE STREQUAL Release))
    add_executable(melonDS WIN32 ${SOURCES_NOGUI_SDL})
else()
    add_executable(melonDS ${SOURCES_NOGUI_SDL})
endif()

target_link_libraries(melonDS ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(melonDS PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(melonDS PRIVATE ${SLIRP_INCLUDE_DIRS})
target_include_directories(melonDS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(melonDS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(melonDS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_link_libraries(melonDS core)

if (BUILD_STATIC)
	target_link_libraries(melonDS -static ${SDL2_STATIC_LIBRARIES} ${SLIRP_STATIC_LIBRARIES})
else()
	target_link_libraries(melonDS ${SDL2_LIBRARIES} ${SLIRP_LIBRARIES})
endif()

if (NOT Iconv_IS_BUILT_IN)
	target_link_libraries(melonDS iconv)
endif()

if (UNIX)
    option(PORTABLE "Make a portable build that looks for its configuration in the current directory" OFF)
    target_link_libraries(melonDS dl)
elseif (WIN32)
    option(PORTABLE "Make a portable build that looks for its configuration in the current directory" ON)
    target_sources(melonDS PUBLIC "${CMAKE_SOURCE_DIR}/melon.rc")

    target_link_libraries(melonDS comctl32 d2d1 dwrite uxtheme ws2_32 iphlpapi gdi32)
    if (BUILD_STATIC)
        target_link_libraries(melonDS imm32 winmm version setupapi -static z zstd)
    else()
        target_link_libraries(melonDS)
    endif()
endif()

if (PORTABLE)
    add_definitions(-DPORTABLE)
endif()

if (APPLE)
    set_target_properties(melonDS PROPERTIES
                         MACOSX_BUNDLE true
                         MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/melonDS.plist
                         OUTPUT_NAME melonDS
                         )

    # Copy icon into the bundle
    target_sources(melonDS PRIVATE "${CMAKE_SOURCE_DIR}/melonDS.icns")
    set_source_files_properties("${CMAKE_SOURCE_DIR}/melonDS.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

endif()

install(FILES ../../../net.kuribo64.melonDS.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES ../../../icon/melon_16x16.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/apps RENAME net.kuribo64.melonDS.png)
install(FILES ../../../icon/melon_32x32.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps RENAME net.kuribo64.melonDS.png)
install(FILES ../../../icon/melon_48x48.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/48x48/apps RENAME net.kuribo64.melonDS.png)
install(FILES ../../../icon/melon_64x64.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps RENAME net.kuribo64.melonDS.png)
install(FILES ../../../icon/melon_128x128.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps RENAME net.kuribo64.melonDS.png)
install(FILES ../../../icon/melon_256x256.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256/apps RENAME net.kuribo64.melonDS.png)
install(TARGETS melonDS BUNDLE DESTINATION ${CMAKE_BINARY_DIR} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
