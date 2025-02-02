cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER gcc)
project(fastaf LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(SRCS
  srcs/fastaf.c
  srcs/ws.c
  srcs/fix.c
  srcs/rest.c
  srcs/dns_resolver.c
  srcs/event_loop.c
  srcs/http_parser.c
  srcs/decompression.c
  srcs/graph.c
  srcs/logger.c
  srcs/signals.c
  srcs/errors.c
  srcs/keys.c
  srcs/fast_ssl.c
  srcs/fixed_point.c
  srcs/message_broker.c
  srcs/str_utils.c
)

add_executable(fastaf ${SRCS})

target_include_directories(fastaf PRIVATE 
  srcs/headers
  /usr/include
)

target_compile_options(fastaf PRIVATE
  -Wall
  -Wextra
  -Werror
  -fwrapv
  -fgnu89-inline
  -D_GNU_SOURCE=1
  -Ofast
  -ffast-math
  -fopt-info-vec-all
  -march=native
  -fomit-frame-pointer
  -flto
)

#TODO static linking
#TODO
target_link_options(fastaf PRIVATE -flto)
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