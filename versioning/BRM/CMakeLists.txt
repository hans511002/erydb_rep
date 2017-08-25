
include_directories( ${ENGINE_COMMON_INCLUDES} )


########### next target ###############

set(brm_LIB_SRCS
    autoincrementmanager.cpp
    brmshmimpl.cpp
    brmtypes.cpp
    copylocks.cpp
    dbrm.cpp
    extentmap.cpp
    lbidresourcegraph.cpp
    logicalpartition.cpp
    mastersegmenttable.cpp
    oidserver.cpp
    resourcenode.cpp
    rgnode.cpp
    rwlockmonitor.cpp
    sessionmanagerserver.cpp
    shmkeys.cpp
    slavedbrmnode.cpp
    slavecomm.cpp
    tablelockserver.cpp
    tracer.cpp
    transactionnode.cpp
    undoable.cpp
    vbbm.cpp
    vss.cpp)

add_library(brm SHARED ${brm_LIB_SRCS})

target_link_libraries(brm ${NETSNMP_LIBRARIES})

set_target_properties(brm PROPERTIES VERSION 1.0.0 SOVERSION 1)

install(TARGETS brm DESTINATION ${ENGINE_LIBDIR} COMPONENT libs)


########### next target ###############

set(controllernode_SRCS masternode.cpp masterdbrmnode.cpp)

add_executable(controllernode ${controllernode_SRCS})

target_link_libraries(controllernode ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${NETSNMP_LIBRARIES})

install(TARGETS controllernode DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############

set(workernode_SRCS slavenode.cpp)

add_executable(workernode ${workernode_SRCS})

target_link_libraries(workernode ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${NETSNMP_LIBRARIES})

install(TARGETS workernode DESTINATION ${ENGINE_BINDIR} COMPONENT platform)
