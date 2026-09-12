add_library(Legolas INTERFACE)
target_include_directories(Legolas INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
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
      $<$<CXX_COMPILER_ID:Clang>:$<$<NOT:$<CONFIG:Debug>>:-march=native>>
    )
  endif()
endif()



