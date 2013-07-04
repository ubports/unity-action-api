set(CORE_SRCS
    plugin.cpp
    qml-action.cpp
    qml-preview-action.cpp
    qml-manager.cpp
    qml-context.cpp
)

# Build everything twice. Since we have only a few
# source files, this is not an issue. If it does
# become one, change tools so they use the plugin
# instead of calling the classes directly.

add_library(unity-action-qml SHARED
${CORE_SRCS}
)
target_link_libraries(unity-action-qml unity-action-qt)

qt5_use_modules(unity-action-qml Qml)

get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)
exec_program(${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_QML" OUTPUT_VARIABLE QT_IMPORTS_DIR)
file(TO_CMAKE_PATH "${QT_IMPORTS_DIR}" QT_IMPORTS_DIR)

install(
  TARGETS unity-action-qml
  ARCHIVE DESTINATION ${QT_IMPORTS_DIR}/Ubuntu/Unity/Action
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${QT_IMPORTS_DIR}/Ubuntu/Unity/Action
)

install(
  FILES qmldir
  DESTINATION ${QT_IMPORTS_DIR}/Ubuntu/Unity/Action
)

# copy qmldir file into build directory for shadow builds
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/qmldir"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )
