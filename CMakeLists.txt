# Project
cmake_minimum_required(VERSION 3.12)
project(doxygenmd)

# Doxygen
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/doxygenmd/bin)
set(DISABLE_DOXYGEN_TESTS 1)
add_subdirectory(ext/doxygen)

# Doxygenmd
configure_file(src/doxygenmd ${CMAKE_CURRENT_BINARY_DIR}/bin/doxygenmd COPYONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/doxygenmd DESTINATION bin)

# Manual
configure_file(src/doxygenmd.1 ${CMAKE_CURRENT_BINARY_DIR}/share/man/man1/doxygenmd.1 COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/man/man1/doxygenmd.1 DESTINATION share/man/man1)

# Tests
enable_testing()
configure_file(tests/in001.cpp ${CMAKE_CURRENT_BINARY_DIR}/in001.cpp COPYONLY)
configure_file(tests/out001.md ${CMAKE_CURRENT_BINARY_DIR}/out001.md COPYONLY)
configure_file(tests/runtest.sh ${CMAKE_CURRENT_BINARY_DIR}/runtest.sh COPYONLY)
add_test(test001 "${CMAKE_CURRENT_BINARY_DIR}/runtest.sh" "001")

# Uninstall
add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/bin/doxygenmd"
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/share/doxygenmd/bin/doxygen"
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/share/man/man1/doxygenmd.1"
)
