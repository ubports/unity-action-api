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

# from www.nondot.org/sabre/Mirrored/libtool-2.1a/libtool_6.html
# Here are a set of rules to help you update your library version information:
#
#    Start with version information of `0:0:0' for each libtool library.
#    Update the version information only immediately before a public release of your
#    software. More frequent updates are unnecessary, and only guarantee that the current
#    interface number gets larger faster.
#
#    If the library source code has changed at all since the last update, then increment
#    revision (`c:r:a' becomes `c:r+1:a').
#
#    If any interfaces have been added, removed, or changed since the last update, increment
#    current, and set revision to 0.

#    If any interfaces have been added since the last public release, then increment age.
#    If any interfaces have been removed since the last public release, then set age to 0. 

set(SO_CURRENT  0)
set(SO_REVISION 0)
set(SO_AGE      0)

set(SO_VERSION ${SO_CURRENT}.${SO_REVISION}.${SO_AGE})

set_target_properties(unity-action-qt PROPERTIES
                      SOVERSION ${SO_VERSION}
)

set(API_VERSION_MAJOR 1)
set(API_VERSION_MINOR 0)

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
