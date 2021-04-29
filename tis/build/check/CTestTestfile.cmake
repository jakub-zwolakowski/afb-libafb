# CMake generated Testfile for 
# Source directory: /home/qba/git/afb-libafb/check
# Build directory: /home/qba/git/afb-libafb/tis/build/check
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(check_check "/home/qba/git/afb-libafb/tis/build/check/tests/check_check")
set_tests_properties(check_check PROPERTIES  _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;501;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(check_check_export "/home/qba/git/afb-libafb/tis/build/check/tests/check_check_export")
set_tests_properties(check_check_export PROPERTIES  _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;502;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_output.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_output.sh")
set_tests_properties(test_output.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;506;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_log_output.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_log_output.sh")
set_tests_properties(test_log_output.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;509;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_xml_output.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_xml_output.sh")
set_tests_properties(test_xml_output.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;512;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_tap_output.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_tap_output.sh")
set_tests_properties(test_tap_output.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;515;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_check_nofork.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_check_nofork.sh")
set_tests_properties(test_check_nofork.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;518;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_check_nofork_teardown.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_check_nofork_teardown.sh")
set_tests_properties(test_check_nofork_teardown.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;521;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
add_test(test_set_max_msg_size.sh "sh" "/home/qba/git/afb-libafb/check/tests/test_set_max_msg_size.sh")
set_tests_properties(test_set_max_msg_size.sh PROPERTIES  WORKING_DIRECTORY "/home/qba/git/afb-libafb/tis/build/check/tests" _BACKTRACE_TRIPLES "/home/qba/git/afb-libafb/check/CMakeLists.txt;524;add_test;/home/qba/git/afb-libafb/check/CMakeLists.txt;0;")
subdirs("doc")
subdirs("lib")
subdirs("src")
subdirs("checkmk")
subdirs("tests")
