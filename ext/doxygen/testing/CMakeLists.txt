if(NOT DISABLE_DOXYGEN_TESTS)
add_custom_target(tests
         COMMENT "Running doxygen tests..."
	 COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/testing/runtests.py --doxygen ${PROJECT_BINARY_DIR}/bin/doxygen --inputdir ${PROJECT_SOURCE_DIR}/testing --outputdir ${PROJECT_BINARY_DIR}/testing
         DEPENDS doxygen
)
add_test(NAME suite
	COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/testing/runtests.py --doxygen $<TARGET_FILE:doxygen> --inputdir ${PROJECT_SOURCE_DIR}/testing --outputdir ${PROJECT_BINARY_DIR}/testing
)
endif()
