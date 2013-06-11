set(TEST_SRCS
    main.cpp
    tst_action.cpp
    tst_previewaction.cpp
    tst_previewrangeparameter.cpp
)

set(testCommand ${CMAKE_CURRENT_BINARY_DIR}/cpptest
    -o ${CMAKE_BINARY_DIR}/cpptest.xml,xunitxml
    -o -,txt)


add_executable(cpptest ${TEST_SRCS})
target_link_libraries(cpptest unity-action-qt)
qt5_use_modules(cpptest Core Test)

add_test(NAME cpp COMMAND ${testCommand})
