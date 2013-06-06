set(TEST_SRCS
    main.cpp
    tst_action.cpp
)

add_executable(cpptest ${TEST_SRCS})
target_link_libraries(cpptest unity-action)
qt5_use_modules(cpptest Core Test)

add_test(cpp cpptest)
