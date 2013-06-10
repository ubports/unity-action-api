set(CORE_SRCS
    unity-action.cpp
    unity-preview-action.cpp
)

set(PUBLIC_HEADER_DIR "${CMAKE_SOURCE_DIR}/include/unity/action")

set(PUBLIC_HEADERS
    ${PUBLIC_HEADER_DIR}/unity-action.h
    ${PUBLIC_HEADER_DIR}/unity-preview-action.h
)

add_library(unity-action-qt SHARED
    ${CORE_SRCS}
    ${PUBLIC_HEADERS} # public headers have to be included here for Q_DECL_EXPORT to work properly
)

qt5_use_modules(unity-action-qt Core)

install(
  TARGETS unity-action-qt
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(unity-action-qt PROPERTIES SOVERSION 0.0.0)
