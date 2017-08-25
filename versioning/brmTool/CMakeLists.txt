
include_directories( ${ENGINE_COMMON_INCLUDES} )

########### next target ###############

set(dbrmctl_SRCS dbrmctl.cpp)

add_executable(dbrmctl ${dbrmctl_SRCS})

target_link_libraries(dbrmctl ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${ENGINE_WRITE_LIBS})

install(TARGETS dbrmctl DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############

set(reset_locks_SRCS reset_locks.cpp)

add_executable(reset_locks ${reset_locks_SRCS})

target_link_libraries(reset_locks ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${ENGINE_WRITE_LIBS})

install(TARGETS reset_locks DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############

set(rollback_SRCS rollback.cpp)

add_executable(rollback ${rollback_SRCS})

target_link_libraries(rollback ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${ENGINE_WRITE_LIBS})

install(TARGETS rollback DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############

set(save_brm_SRCS save_brm.cpp blockresolutionmanager.cpp)

add_executable(save_brm ${save_brm_SRCS})

target_link_libraries(save_brm ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${ENGINE_WRITE_LIBS})

install(TARGETS save_brm DESTINATION ${ENGINE_BINDIR} COMPONENT platform)


########### next target ###############

set(load_brm_SRCS load_brm.cpp blockresolutionmanager.cpp)

add_executable(load_brm ${load_brm_SRCS})

target_link_libraries(load_brm ${ENGINE_LDFLAGS} ${ENGINE_OAM_LIBS} ${ENGINE_EXEC_LIBS} ${ENGINE_WRITE_LIBS})

install(TARGETS load_brm DESTINATION ${ENGINE_BINDIR} COMPONENT platform)

