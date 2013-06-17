set(TEST_SRCS
    main.cpp
    tst_action.cpp
    tst_previewaction.cpp
    tst_previewrangeparameter.cpp
    tst_menuitem.cpp
    tst_actioncontext.cpp
    tst_actionmanager.cpp
)

set(testCommand ${CMAKE_CURRENT_BINARY_DIR}/cpptest
    -o ${CMAKE_BINARY_DIR}/cpptest.xml,xunitxml
    -o -,txt)

pkg_search_module(GIO REQUIRED gio-2.0)
include_directories(${GIO_INCLUDE_DIRS})

add_executable(cpptest ${TEST_SRCS})
target_link_libraries(cpptest unity-action-qt ${GIO_LIBRARIES})
qt5_use_modules(cpptest Core Test)

add_test(NAME cpp COMMAND ${testCommand})
