add_subdirectory(cpp)
add_subdirectory(qml)

add_custom_target(doc)
add_dependencies(doc cppdoc qmldoc)
