find_package(Qt6 REQUIRED Quick)

file(GLOB_RECURSE src "*.cpp" "*.hpp")
list(REMOVE_ITEM src "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(ui_ STATIC ${src})
set_target_properties(ui_ PROPERTIES AUTOMOC ON)
target_link_libraries(ui_ PUBLIC
    Qt6::Quick

    craie::common
    craie::core
)

qt_add_qml_module(ui_
    URI craie
    RESOURCE_PREFIX "qml"
    QML_FILES
        "main.qml"
    DEPENDENCIES
        QtQuick
)

add_executable(ui "main.cpp")
set_target_properties(ui PROPERTIES OUTPUT_NAME "craie")
target_link_libraries(ui PRIVATE ui_ ui_plugin)
