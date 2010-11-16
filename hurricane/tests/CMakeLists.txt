
   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane )
        add_executable ( htest HTest.cpp )
 target_link_libraries ( htest hurricane ${Boost_LIBRARIES} )
