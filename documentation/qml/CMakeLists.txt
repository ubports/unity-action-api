# add a target to generate API documentation with qdoc
if(DEFINED ENV{DEB_BUILD_MULTIARCH})
  set(QDOC_EXECUTABLE "/usr/lib/$ENV{DEB_BUILD_MULTIARCH}/qt5/bin/qdoc")
else()
  FIND_PROGRAM(QDOC_EXECUTABLE qdoc)
endif()
if(QDOC_EXECUTABLE)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/unity-action-qml.qdocconf.in ${CMAKE_CURRENT_BINARY_DIR}/unity-action-qml.qdocconf @ONLY)
add_custom_target(qmldoc
${QDOC_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/unity-action-qml.qdocconf
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating QML API documentation with qdoc" VERBATIM
)

# copy stylesheet files into build directory for shadow builds
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/css"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    )

install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/
  DESTINATION ${CMAKE_INSTALL_DOCDIR}/qml/
)

endif(QDOC_EXECUTABLE)


