find_package(Boost REQUIRED json)

file(GLOB_RECURSE src "*.cpp")
add_library(core ${src})
add_library(craie::core ALIAS core)
target_link_libraries(core PUBLIC
    Boost::headers Boost::json

    craie::common
)
