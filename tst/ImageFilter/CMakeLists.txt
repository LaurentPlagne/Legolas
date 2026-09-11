add_executable(ImageFilter ImageFilter.cxx)
configure_file(lena.bmp lena.bmp COPYONLY)
target_link_libraries(ImageFilter Legolas)
configure_file(lena.bmp lena.bmp COPYONLY)

