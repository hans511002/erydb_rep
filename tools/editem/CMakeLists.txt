
include_directories( ${ENGINE_COMMON_INCLUDES} )


########### next target ###############
set(editem_SRCS editem.cpp)
add_executable(editem ${editem_SRCS})
target_link_libraries(editem ${ENGINE_LDFLAGS} ${NETSNMP_LIBRARIES} ${ENGINE_EXEC_LIBS})
install(TARGETS editem DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############
set(erytest_SRCS erytest.cpp)
add_executable(erytest ${erytest_SRCS})
target_link_libraries(erytest ${ENGINE_LDFLAGS} ${NETSNMP_LIBRARIES} ${ENGINE_EXEC_LIBS})
install(TARGETS erytest DESTINATION ${ENGINE_BINDIR} COMPONENT platform)
