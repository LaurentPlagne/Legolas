enable_testing()

add_subdirectory(MultiThomas)
add_subdirectory(MultiThomasExample)
add_subdirectory(StaticArrays)
add_subdirectory(ImageFilter)
#add_subdirectory(TensorTranspose)
add_subdirectory(MultiPrec)

add_test(NAME MultiThomasExample COMMAND MultiThomasExample)
add_test(NAME StaticArrays COMMAND StaticArrays)
add_test(NAME MultiPrec COMMAND MultiPrec)
