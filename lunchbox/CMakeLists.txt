
# Copyright (c) 2012-2015 Stefan Eilemann <eile@eyescale.ch>

include(files.cmake)

set(LUNCHBOX_PUBLIC_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
list(APPEND LUNCHBOX_LINK_LIBRARIES PUBLIC ${Boost_LIBRARIES} Servus
                                    PRIVATE ${PTHREAD_LIBRARIES})

if(WIN32)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ws2_32 mswsock imagehlp)
endif(WIN32)
if(LINUX)
  list(APPEND LUNCHBOX_LINK_LIBRARIES dl rt)
endif()
if(HWLOC_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ${hwloc_LIBRARIES})
endif()
if(DNSSD_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ${DNSSD_LIBRARIES})
endif()
if(AVAHI-CLIENT_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ${avahi-client_LIBRARIES})
endif()
if(MPI_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ${MPI_LIBRARIES})
endif()
if(LEVELDB_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES ${LEVELDB_LIBRARIES})
endif()
if(SKV_FOUND)
  list(APPEND LUNCHBOX_LINK_LIBRARIES skv_client fxlogger)
endif()

common_library(Lunchbox)

target_include_directories(Lunchbox PUBLIC ${Boost_INCLUDE_DIRS})
source_group(\\ FILES CMakeLists.txt)
source_group(lunchbox FILES ${LUNCHBOX_PUBLIC_HEADERS} ${LUNCHBOX_HEADERS}
  ${LUNCHBOX_SOURCES})
