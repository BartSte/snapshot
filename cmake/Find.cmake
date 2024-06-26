# This file contains helper functions for finding libraries.

# Functon wrapper around the `FindFFmpegPatched.cmake` script.
function(find_ffmpeg)
  include(${CMAKE_SOURCE_DIR}/cmake/FindFFmpegPatched.cmake)
endfunction()

# Finds static libraries for `LIB_NAME` and sets the variable `OUT` to the
# absolute path of the library. If the library is found, the
# FOUND_{LIB_NAME}_STATIC is set.
function(find_static_libraries LIB_NAME OUT)
  if(WIN32 OR MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
  elseif(UNIX)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  endif()

  find_library(FOUND_${LIB_NAME}_STATIC ${LIB_NAME})

  if(FOUND_${LIB_NAME}_STATIC)
    get_filename_component(ABS_FILE ${FOUND_${LIB_NAME}_STATIC} ABSOLUTE)
  else()
    message(SEND_ERROR "Unable to find library ${LIB_NAME}")
  endif()

  set(${OUT}
      ${ABS_FILE}
      PARENT_SCOPE)
endfunction()

# This function was copied from the `sora-cpp-sdk` project located at: -
# https://github.com/shiguredo/sora-cpp-sdk/blob/develop/cmake/FindLibdrm.cmake
# The script not modified.
function(find_libdrm)
  find_package(Libdrm QUIET)
  find_package(PkgConfig)

  pkg_check_modules(Libdrm libdrm)
  if(Libdrm_FOUND)
    if(NOT TARGET Libdrm::drm)
      add_library(Libdrm::drm UNKNOWN IMPORTED)

      set_target_properties(
        Libdrm::drm
        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Libdrm_INCLUDE_DIRS}"
                   IMPORTED_LOCATION "${Libdrm_LINK_LIBRARIES}")
    endif()
  endif()
endfunction()

# Sets the variable `libva_long` to the long name of the variable `libva_short`.
# For example, if `libva_short` is `va_drm`, `libva_long` will be set to
# `LIBVA_DRM_LIBRARY`.
macro(_short2long libva_short)
  set(libva_long "LIBVA_${libva_short}_LIBRARY")
  string(REGEX REPLACE "va(-)?" "" libva_long "${libva_long}")
  string(REGEX REPLACE "__" "_" libva_long "${libva_long}")
  string(TOUPPER "${libva_long}" libva_long)
endmacro()

# cmake-format: off
# Finds the following vaapi libraries:
# - libva
# - libva_drm
# - libva_wayland
# - libva_x11
#
# If the libraries are found, the following variables are set with the absolute
# path of the library:
# - LIBVA_LIBRARY
# - LIBVA_DRM_LIBRARY
# - LIBVA_WAYLAND_LIBRARY
# - LIBVA_X11_LIBRARY
#
# Also, the following variables is set to the include directory of libva:
# - LIBVA_INCLUDE_DIR
#
# Finally, the following variables is set to TRUE if all the libraries are
# found:
# - Libva_FOUND
#
# If one of the libraries is not found, a error is printed and the function
# returns.
# cmake-format: on
function(find_libva)
  set(libva_shorts va va-drm va-wayland va-x11)
  set(libva_longs LIBVA_LIBRARY LIBVA_DRM_LIBRARY LIBVA_WAYLAND_LIBRARY
                  LIBVA_X11_LIBRARY)
  if(LIBVA_DIR)
    message(STATUS "LIBVA_DIR: ${LIBVA_DIR}")
    set(find_args PATHS ${LIBVA_DIR} NO_DEFAULT_PATH)
  endif()

  find_libdrm()
  find_path(
    LIBVA_INCLUDE_DIR
    NAMES va/va.h
    PATH_SUFFIXES include ${find_args})
  message(STATUS "LIBVA_INCLUDE_DIR: ${LIBVA_INCLUDE_DIR}")

  foreach(libva_short ${libva_shorts})
    _short2long(${libva_short}) # creates libva_long

    find_library(
      ${libva_long}
      NAMES ${libva_short}
      PATH_SUFFIXES lib ${find_args})
    add_library(Libva::${libva_short} UNKNOWN IMPORTED)
    set_target_properties(
      Libva::${libva_short}
      PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LIBVA_INCLUDE_DIR}"
                 IMPORTED_LOCATION ${${libva_long}})

    message(STATUS "${libva_long} is at: ${${libva_long}}")
  endforeach()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Libva DEFAULT_MSG ${libva_longs})
endfunction()

# cmake-format: off
# Finds the following Qt6 libraries:
# - Qt6::Widgets
# - Qt6::Multimedia
# - Qt6::MultimediaWidgets
# - Qt6::WaylandClient
# - FFmpeg libraries (indirectly)
# It sets the following variables:
# - QT6_LIB_TYPE: Type of the Qt6 library (STATIC_LIBRARY or SHARED_LIBRARY)
# - QT6_TOP_DIR: Qt6 top directory
# cmake-format: on
function(find_qt6_base_and_multimedia)
  # Also finds FFmpeg libraries see FindFFmpegPatched.cmake for the variables
  # that are created
  find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets
                                       WaylandClient)
  get_target_property(QT6_LIB_TYPE Qt6::Widgets TYPE)
  set(QT6_LIB_TYPE
      ${QT6_LIB_TYPE}
      CACHE STRING "Type of the Qt6 library (STATIC_LIBRARY or SHARED_LIBRARY)")

  set(qt6_top_dir_relative "${Qt6_DIR}/../../../")
  cmake_path(NORMAL_PATH qt6_top_dir_relative OUTPUT_VARIABLE qt6_top_dir)
  set(QT6_TOP_DIR
      "${qt6_top_dir}"
      CACHE PATH "Qt6 top directory")

  message(STATUS "Qt6 was found at ${Qt6_DIR}")
  message(STATUS "Qt6 its top directory is ${QT6_TOP_DIR}")
  message(STATUS "Qt6 library type is ${QT6_LIB_TYPE}")
  message(STATUS "FFmpeg libraries are: ${FFMPEG_LIBRARIES}")
endfunction()
