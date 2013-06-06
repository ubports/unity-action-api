add_custom_target(qmlunittests)

# add_qml_test macro
include(QmlTest)

set(qmltest_DEFAULT_IMPORT_PATHS
  ${CMAKE_CURRENT_BINARY_DIR}/../../qml
)

set(qmltest_DEFAULT_TARGETS qmlunittests)
set(qmltest_DEFAULT_NO_ADD_TEST FALSE)
set(qmltest_DEFAULT_PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
add_qml_test(. api)

