set(EXAMPLE_SRCS
    mail-app.cpp
)

add_executable(mail-app ${EXAMPLE_SRCS})
target_link_libraries(mail-app unity-action-qt)
qt5_use_modules(mail-app Core Widgets)
