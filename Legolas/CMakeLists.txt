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



