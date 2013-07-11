add_subdirectory(cpp)
add_subdirectory(qml)

add_custom_target(doc ALL)
add_dependencies(doc cppdoc qmldoc)
