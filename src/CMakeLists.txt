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

set_target_properties(unity-action-qt PROPERTIES SOVERSION 0.0.0)

#install the headers
# TODO: add library version to the path
# TODO: add .pc file
#install(FILES ${PUBLIC_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/unity/action" )
