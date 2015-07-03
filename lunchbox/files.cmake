
# Copyright (c) 2012-2014 Stefan Eilemann <eile@eyescale.ch>

set(LUNCHBOX_PUBLIC_HEADERS
  ${COMMON_INCLUDES}
  algorithm.h
  any.h
  anySerialization.h
  array.h
  atomic.h
  bitOperation.h
  buffer.h
  buffer.ipp
  clock.h
  compiler.h
  condition.h
  daemon.h
  debug.h
  dso.h
  file.h
  floatMap.h
  floatMap.ipp
  future.h
  futureFunction.h
  hash.h
  indexIterator.h
  init.h
  launcher.h
  lfQueue.h
  lfQueue.ipp
  lfVector.h
  lfVector.ipp
  lfVectorIterator.h
  lock.h
  lockable.h
  log.h
  memoryMap.h
  monitor.h
  mpi.h
  mtFloatMap.ipp
  mtQueue.h
  mtQueue.ipp
  nonCopyable.h
  omp.h
  os.h
  perfLogger.h
  perThread.h
  perThread.ipp
  perThreadRef.h
  persistentMap.h
  plugin.h
  pluginFactory.h
  pluginFactory.ipp
  pluginRegisterer.h
  pool.h
  readyFuture.h
  refPtr.h
  referenced.h
  request.h
  requestHandler.h
  result.h
  rng.h
  scopedMutex.h
  serializable.h
  sleep.h
  spinLock.h
  stdExt.h
  thread.h
  threadID.h
  timedLock.h
  tls.h
  types.h
  unorderedIntervalSet.h
  unorderedIntervalSet.ipp
  visitorResult.h
  )
if(NOT LUNCHBOX_BUILD_V2_API)
  list(APPEND LUNCHBOX_PUBLIC_HEADERS
    servus.h
    uint128_t.h
    uri.h
    )
endif()

if(Boost_VERSION VERSION_LESS 1.43.0)
  list(APPEND LUNCHBOX_PUBLIC_HEADERS factory.hpp)
endif()

set(LUNCHBOX_HEADERS
  detail/threadID.h
  leveldb/persistentMap.h
  skv/persistentMap.h
  time.h
)

set(LUNCHBOX_SOURCES
  ${COMMON_SOURCES}
  any.cpp
  atomic.cpp
  clock.cpp
  condition.cpp
  condition_w32.ipp
  debug.cpp
  dso.cpp
  file.cpp
  init.cpp
  launcher.cpp
  lock.cpp
  log.cpp
  memoryMap.cpp
  mpi.cpp
  omp.cpp
  os.cpp
  persistentMap.cpp
  referenced.cpp
  requestHandler.cpp
  rng.cpp
  sleep.cpp
  spinLock.cpp
  thread.cpp
  threadID.cpp
  timedLock.cpp
  tls.cpp
  )
