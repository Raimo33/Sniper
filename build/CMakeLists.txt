cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER gcc)
project(fastaf LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(SRCS
  fastaf.c
  ws.c
  fix.c
  rest.c
  dns_resolver.c
  event_loop.c
  http_parser.c
  fix_parser.c
  decompression.c
  graph.c
  logger.c
  signals.c
  errors.c
  keys.c
  fast_ssl.c
  fixed_point.c
  message_broker.c
  str_utils.c
)
list(TRANSFORM SRCS PREPEND "srcs/")

add_executable(fastaf ${SRCS})

target_include_directories(fastaf PRIVATE 
  srcs/headers
  /usr/include
)

target_compile_options(fastaf PRIVATE
  -Wall -Wextra -Werror
  -fwrapv
  -fgnu89-inline
  -D_GNU_SOURCE=1
  -O3
  -march=native
  -flto
  -fomit-frame-pointer
)

target_link_options(fastaf PRIVATE -flto -static)
target_link_libraries(fastaf PRIVATE ssl crypto z yyjson)

set(VALGRIND_SUPP "${CMAKE_SOURCE_DIR}/valgrind.supp")
set(VALGRIND_LOG "valgrind.log")
set(CACHEGRIND_LOG "cachegrind.log")
set(CALLGRIND_LOG "callgrind.log")
set(MASSIF_LOG "massif.log")

set(VALGRIND_FLAGS 
  "--leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=${VALGRIND_SUPP} --log-file=${VALGRIND_LOG}"
)
set(CACHEGRIND_FLAGS 
  "--tool=cachegrind --cache-sim=yes --branch-sim=yes --dump-instr=yes --cachegrind-out-file=${CACHEGRIND_LOG}"
)
set(CALLGRIND_FLAGS "--tool=callgrind --callgrind-out-file=${CALLGRIND_LOG}")
set(MASSIF_FLAGS "--tool=massif --massif-out-file=${MASSIF_LOG}")

add_custom_target(leaks
  COMMAND valgrind ${VALGRIND_FLAGS} ./fastaf
  DEPENDS fastaf
  COMMENT "Running memory leak check with Valgrind"
)

add_custom_target(memory
  COMMAND valgrind ${CACHEGRIND_FLAGS} ./fastaf
  COMMAND cg_annotate ${CACHEGRIND_LOG}
  COMMAND valgrind ${CALLGRIND_FLAGS} ./fastaf
  COMMAND callgrind_annotate ${CALLGRIND_LOG}
  COMMAND valgrind ${MASSIF_FLAGS} ./fastaf
  COMMAND ms_print ${MASSIF_LOG}
  DEPENDS fastaf
  COMMENT "Running comprehensive memory analysis"
)