cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(QGeoView LANGUAGES C CXX)

option(BUILD_EXAMPLES "Build examples" OFF)

# Check if QT_VERSION_MAJOR is defined
if (NOT DEFINED QT_VERSION_MAJOR)
    message(FATAL_ERROR "Please specify Qt version")
else()
    set(QT_VERSION "${QT_VERSION_MAJOR}")
endif ()

find_package(GDAL CONFIG)

add_subdirectory(lib)
add_subdirectory(samples/shared)

if (${BUILD_EXAMPLES})
  message(STATUS "Enabled building of examples")
  add_subdirectory(samples/basic)
  add_subdirectory(samples/background)
  add_subdirectory(samples/widgets)
  add_subdirectory(samples/flags)
  add_subdirectory(samples/layers)
  add_subdirectory(samples/raster)
  add_subdirectory(samples/fun)
  add_subdirectory(samples/10000)
  add_subdirectory(samples/debug)
  add_subdirectory(samples/performance)
  add_subdirectory(samples/custom-tiles)
  add_subdirectory(samples/moving-objects)
  add_subdirectory(samples/mouse-actions)
  add_subdirectory(samples/camera-actions)
  add_subdirectory(samples/drag-and-drop)
  add_subdirectory(samples/gps_sample)

  if(GDAL_FOUND)
    add_subdirectory(samples/gdal-shapefile)
  endif()
else ()
  message(STATUS "Disabled building of examples")
endif ()
