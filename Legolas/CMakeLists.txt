include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(Legolas INTERFACE)
add_library(Legolas::Legolas ALIAS Legolas)

target_include_directories(Legolas INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Legolas/include>
)
target_compile_definitions(Legolas INTERFACE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:_GNU_SOURCE>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:_POSIX_C_SOURCE=200809L>
)
target_link_libraries(Legolas INTERFACE Threads::Threads)

# Header-only libraries carry no build flags of their own. When the library is
# consumed through FetchContent/add_subdirectory, the consumer's configuration
# may be a default (non optimized) one and silently produce scalar code.
# LEGOLAS_INTERFACE_OPTIMIZE opts into propagating the optimization flags
# required to actually get SIMD out of the headers.
option(LEGOLAS_INTERFACE_OPTIMIZE
  "Propagate optimization flags (-O3, -march=native) to Legolas consumers" OFF)

if (LEGOLAS_INTERFACE_OPTIMIZE)
  if (MSVC)
    target_compile_options(Legolas INTERFACE $<$<NOT:$<CONFIG:Debug>>:/O2>)
  else()
    target_compile_options(Legolas INTERFACE
      $<$<NOT:$<CONFIG:Debug>>:-O3>
      $<$<CXX_COMPILER_ID:GNU>:$<$<NOT:$<CONFIG:Debug>>:-march=native>>
    )
    if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
      target_compile_options(Legolas INTERFACE
        $<$<CXX_COMPILER_ID:Clang>:$<$<NOT:$<CONFIG:Debug>>:-march=native>>
      )
    endif()
  endif()
endif()

# ---------------------------------------------------------------------------
# Installation (100% header-only)
#
#   cmake --install build --prefix <prefix>
#
# Consumer side:
#   find_package(Legolas CONFIG REQUIRED)
#   target_link_libraries(app PRIVATE Legolas::Legolas)   # or `Legolas`
# ---------------------------------------------------------------------------
install(TARGETS Legolas EXPORT LegolasTargets)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Legolas"
  FILES_MATCHING PATTERN "*.hxx" PATTERN "*.h"
  PATTERN "cmake" EXCLUDE
  PATTERN "tst" EXCLUDE
  PATTERN "shaders" EXCLUDE
)

install(EXPORT LegolasTargets
  FILE LegolasTargets.cmake
  NAMESPACE Legolas::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Legolas"
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LegolasConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/LegolasConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Legolas"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/LegolasConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/LegolasConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/LegolasConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Legolas"
)



