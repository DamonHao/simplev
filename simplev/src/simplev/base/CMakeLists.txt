set(base_SRCS
  # AsyncLogging.cc
  # Condition.cc
  CountDownLatch.cc
  # Date.cc
  Exception.cc
  # FileUtil.cc
  # LogFile.cc
  Logging.cc
  LogStream.cc
  # ProcessInfo.cc
  Timestamp.cc
  # TimeZone.cc
  Thread.cc
  # ThreadPool.cc
  )

add_library(simplev_base ${base_SRCS})
target_link_libraries(simplev_base pthread rt)

add_library(simplev_base_cpp11 ${base_SRCS})
target_link_libraries(simplev_base_cpp11 pthread rt)
set_target_properties(simplev_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS simplev_base DESTINATION lib)
install(TARGETS simplev_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/simplev/base)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#add_subdirectory(test)
#endif()
