set(EXAMPLE_SRCS
    example.cpp
    mainwindow.cpp
)

add_executable(example ${EXAMPLE_SRCS})
target_link_libraries(example unity-action-qt)
qt5_use_modules(example Core Widgets)

