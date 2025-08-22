find_package(Boost REQUIRED)

file(GLOB_RECURSE src "*.cpp")
add_library(core ${src})
add_library(craie::core ALIAS core)
target_link_libraries(core PUBLIC
    Boost::headers

    craie::common
)
