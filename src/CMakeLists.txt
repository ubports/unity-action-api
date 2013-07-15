set(CORE_SRCS
    unity-action.cpp
    unity-preview-action.cpp
    unity-preview-parameter.cpp
    unity-preview-range-parameter.cpp
    unity-menu-item.cpp
    unity-action-manager.cpp
    unity-action-context.cpp
)

set(PUBLIC_HEADER_DIR "${CMAKE_SOURCE_DIR}/include/unity/action")

set(PUBLIC_HEADERS
    ${PUBLIC_HEADER_DIR}/Action
    ${PUBLIC_HEADER_DIR}/unity-action.h
    ${PUBLIC_HEADER_DIR}/PreviewAction
    ${PUBLIC_HEADER_DIR}/unity-preview-action.h
    ${PUBLIC_HEADER_DIR}/PreviewParameter
    ${PUBLIC_HEADER_DIR}/unity-preview-parameter.h
    ${PUBLIC_HEADER_DIR}/PreviewRangeParameter
    ${PUBLIC_HEADER_DIR}/unity-preview-range-parameter.h
#    Don't publish MenuItem yet as we don't have any consumers
#    ${PUBLIC_HEADER_DIR}/MenuItem
    ${PUBLIC_HEADER_DIR}/unity-menu-item.h
    ${PUBLIC_HEADER_DIR}/ActionManager
    ${PUBLIC_HEADER_DIR}/unity-action-manager.h
    ${PUBLIC_HEADER_DIR}/ActionContext
    ${PUBLIC_HEADER_DIR}/unity-action-context.h
)

include_directories(${HUD_INCLUDE_DIRS})

add_library(unity-action-qt SHARED
    ${CORE_SRCS}
    ${PUBLIC_HEADERS} # public headers have to be included here for Q_DECL_EXPORT to work properly
)
target_link_libraries (unity-action-qt ${HUD_LIBRARIES})

qt5_use_modules(unity-action-qt Core)

install(
  TARGETS unity-action-qt
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


set(API_VERSION_MAJOR 1)
set(API_VERSION_MINOR 0)

set(SO_VERSION ${API_VERSION_MAJOR})

set_target_properties(unity-action-qt PROPERTIES
                      SOVERSION ${SO_VERSION}
)

set(PC_FILE_TARGET "${CMAKE_CURRENT_BINARY_DIR}/unity-action-qt-${API_VERSION_MAJOR}.pc")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}/unity-action-qt-${API_VERSION_MAJOR}")
set(ABSOLUTE_SO_FILE "${CMAKE_INSTALL_FULL_LIBDIR}/libunity-action-qt.so.${SO_VERSION}")
configure_file("unity-action-qt.pc.in" ${PC_FILE_TARGET} @ONLY)
install(
  FILES ${PC_FILE_TARGET}
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

#install the headers
install(FILES ${PUBLIC_HEADERS}
        DESTINATION "${includedir}/unity/action")
