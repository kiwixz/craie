find_package(Boost REQUIRED)

file(GLOB_RECURSE src "*.cpp")
add_library(common ${src})
add_library(craie::common ALIAS common)
target_link_libraries(common PUBLIC
    Boost::headers
)
