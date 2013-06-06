set(CORE_SRCS
    plugin.cpp
)

# Build everything twice. Since we have only a few
# source files, this is not an issue. If it does
# become one, change tools so they use the plugin
# instead of calling the classes directly.

add_library(unity-action-qml SHARED
${CORE_SRCS}
)
target_link_libraries(unity-action-qml unity-action)

qt5_use_modules(unity-action-qml Qml)

install(
  TARGETS unity-action-qml
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)

# copy qmldir file into build directory for shadow builds
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/qmldir"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )
