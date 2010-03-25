# build the pypyffi toy application
# (eventually this might go into a seperate project?)
add_executable(pypyffi pypyffi.cpp)
target_link_libraries(pypyffi ${PYTHON_LIBRARIES} ${Boost_PYTHON_LIBRARY} pyffi)

# recurse further
add_subdirectory(pyffi)
