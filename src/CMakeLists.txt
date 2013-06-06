set(CORE_SRCS
    unity-action.cpp
)

# Build everything twice. Since we have only a few
# source files, this is not an issue. If it does
# become one, change tools so they use the plugin
# instead of calling the classes directly.

add_library(unity-action SHARED
${CORE_SRCS}
)

qt5_use_modules(unity-action Qml)

install(
  TARGETS unity-action
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)
