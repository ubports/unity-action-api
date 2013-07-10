set(EXAMPLE_SRCS
    preview-action.cpp
)

add_executable(preview-action ${EXAMPLE_SRCS})
target_link_libraries(preview-action unity-action-qt)
qt5_use_modules(preview-action Core Widgets)

