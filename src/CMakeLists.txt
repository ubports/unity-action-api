set(CORE_SRCS
    unity-action.cpp
    unity-preview-action.cpp
)

# Build everything twice. Since we have only a few
# source files, this is not an issue. If it does
# become one, change tools so they use the plugin
# instead of calling the classes directly.

add_library(unity-action-qt SHARED
${CORE_SRCS}
)

qt5_use_modules(unity-action-qt Core)

install(
  TARGETS unity-action-qt
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(unity-action-qt PROPERTIES SOVERSION 0.0.0)
