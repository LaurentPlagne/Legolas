include_directories(../../)
include_directories(../../Legolas/include)
add_executable(ImageFilter ImageFilter.cxx)
configure_file(lena.bmp lena.bmp COPYONLY)
target_link_libraries(ImageFilter Legolas ${TBB_LIBRARIES})
configure_file(lena.bmp lena.bmp COPYONLY)

