# build _pyffi extension module
add_library(_pyffi SHARED
    _pyffi.cpp)
set_target_properties(_pyffi PROPERTIES PREFIX "")
target_link_libraries(_pyffi ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARY} pyffi)

# copy pure python files to build dir (for testing)
add_custom_command(
    TARGET _pyffi
    COMMAND
        ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py .
)

add_subdirectory(object_models)
