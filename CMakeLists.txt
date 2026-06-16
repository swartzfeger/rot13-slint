cmake_minimum_required(VERSION 3.21)

project(rot13_slint VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Slint QUIET)

if (NOT Slint_FOUND)
    message(STATUS "Slint was not found locally. Fetching Slint release/1 with CMake FetchContent.")
    include(FetchContent)
    FetchContent_Declare(
        Slint
        GIT_REPOSITORY https://github.com/slint-ui/slint.git
        GIT_TAG release/1
        SOURCE_SUBDIR api/cpp
    )
    FetchContent_MakeAvailable(Slint)
endif()

set(rot13_sources src/main.cpp)

if (WIN32)
    enable_language(RC)
    list(APPEND rot13_sources resources/rot13.rc)
endif()

add_executable(rot13-slint ${rot13_sources})

set_target_properties(rot13-slint PROPERTIES OUTPUT_NAME "Tabula")
set_property(TARGET rot13-slint PROPERTY SLINT_EMBED_RESOURCES embed-files)

if (WIN32)
    set_target_properties(rot13-slint PROPERTIES WIN32_EXECUTABLE TRUE)
    target_include_directories(rot13-slint PRIVATE resources)
endif()

if (APPLE)
    set_source_files_properties(resources/rot13.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(rot13-slint PRIVATE resources/rot13.icns)

    set_target_properties(rot13-slint PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Tabula"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.swartzfeger.rot13-slint"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_ICON_FILE "rot13.icns"
    )
endif()

target_link_libraries(rot13-slint PRIVATE Slint::Slint)

slint_target_sources(rot13-slint ui/app-window.slint)

if (APPLE)
    add_custom_command(
        TARGET rot13-slint
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DAPP_EXECUTABLE="$<TARGET_FILE:rot13-slint>"
            -DBUNDLE_FRAMEWORKS_DIR="$<TARGET_BUNDLE_CONTENT_DIR:rot13-slint>/Frameworks"
            -DSLINT_DYLIB="$<TARGET_FILE:slint_cpp-shared>"
            -DSLINT_DYLIB_NAME="$<TARGET_FILE_NAME:slint_cpp-shared>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BundleMacOS.cmake"
    )
endif()

if (WIN32)
    add_custom_command(
        TARGET rot13-slint
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:rot13-slint> $<TARGET_FILE_DIR:rot13-slint>
        COMMAND_EXPAND_LISTS
    )

    if (TARGET slint_cpp-shared)
        add_custom_command(
            TARGET rot13-slint
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:slint_cpp-shared> $<TARGET_FILE_DIR:rot13-slint>
        )
    endif()
endif()
