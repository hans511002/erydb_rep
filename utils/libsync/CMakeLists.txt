
include_directories( ${ENGINE_COMMON_INCLUDES} )
 

########### next target ###############

set(sync_LIB_SRCS syncData.cpp  )

add_library(sync SHARED ${sync_LIB_SRCS})

add_dependencies(sync messageqcpp)

set_target_properties(sync PROPERTIES VERSION 1.0.0 SOVERSION 1)

install(TARGETS sync DESTINATION ${ENGINE_LIBDIR})

