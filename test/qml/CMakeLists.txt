add_custom_target(qmlunittests)
add_custom_target(qmlunittests_1_1)

find_program(qmltestrunner_exe qmltestrunner)

if(NOT qmltestrunner_exe)
  msg(FATAL_ERROR "Could not locate qmltestrunner.")
endif()

set(qmltest_command
  dbus-test-runner -t env -p "QT_QPA_PLATFORM=minimal"
  -p ${qmltestrunner_exe} -p -input -p ${CMAKE_CURRENT_SOURCE_DIR}/tst_api.qml
  -p -import -p ${CMAKE_CURRENT_BINARY_DIR}/../../qml
  -p -o -p ${CMAKE_BINARY_DIR}/testapi.xml,xunitxml
  -p -o -p -,txt
)

add_custom_target(qmltest_api ${qmltest_command})
add_dependencies(qmlunittests qmltest_api)
add_test(qmlunittests ${qmltest_command})

set(qmltest1_1_command
  dbus-test-runner -t env -p "QT_QPA_PLATFORM=minimal"
  -p ${qmltestrunner_exe} -p -input -p ${CMAKE_CURRENT_SOURCE_DIR}/tst_api1.1.qml
  -p -import -p ${CMAKE_CURRENT_BINARY_DIR}/../../qml
  -p -o -p ${CMAKE_BINARY_DIR}/testapi.xml,xunitxml
  -p -o -p -,txt
)

add_custom_target(qmltest_1_1_api ${qmltest1_1_command})
add_dependencies(qmlunittests_1_1 qmltest_1_1_api)
add_test(qmlunittests_1_1 ${qmltest1_1_command})
