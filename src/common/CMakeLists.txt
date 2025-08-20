file(GLOB_RECURSE src "*.cpp")
add_library(common ${src})
add_library(craie::common ALIAS common)
