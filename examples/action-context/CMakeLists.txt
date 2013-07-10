set(EXAMPLE_SRCS
    action-context.cpp
)

add_executable(action-context ${EXAMPLE_SRCS})
target_link_libraries(action-context unity-action-qt)
qt5_use_modules(action-context Core Widgets)

