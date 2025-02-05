cmake_minimum_required(VERSION 3.3)
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
  event_loop.c
  http_serializer.c
  fix_serializer.c
  decompression.c
  graph.c
  logger.c
  signals.c
  errors.c
  keys.c
  fast_ssl.c
  fixed_point.c
  message_broker.c
  string_utils.c
  system_tweaks.c
  wrappers.c
)
list(TRANSFORM SRCS PREPEND "srcs/")

add_executable(fastaf ${SRCS})

target_include_directories(fastaf PRIVATE 
  srcs/headers
  /usr/include
)

link_directories(/usr/lib)

target_compile_options(fastaf PRIVATE
  -Wall -Wextra -Werror
  -fwrapv
  -fgnu89-inline
  -g #TODO remove
  -D_GNU_SOURCE=1
  -fno-strict-aliasing
  -Ofast
  -march=native
  -flto
  -fno-plt
  -fno-semantic-interposition
  -fno-asynchronous-unwind-tables
  -fomit-frame-pointer
  -funroll-loops
  # -floop-nest-optimize
)

target_link_options(fastaf PRIVATE
  -flto
  -static
)

target_link_libraries(fastaf PRIVATE
  ssl
  crypto
  z
  yyjson
  xxhash
)

set(VALGRIND_SUPP ${CMAKE_SOURCE_DIR}/valgrind.supp)
set(VALGRIND_LOG ${CMAKE_BINARY_DIR}/valgrind.log)
set(CACHEGRIND_LOG ${CMAKE_BINARY_DIR}/cachegrind.out)
set(CALLGRIND_LOG ${CMAKE_BINARY_DIR}/callgrind.out)
set(MASSIF_LOG ${CMAKE_BINARY_DIR}/massif.out)

set(VALGRIND_FLAGS 
  --leak-check=full
  --show-leak-kinds=all
  --track-origins=yes
  --track-fds=yes
  --suppressions=${VALGRIND_SUPP}
  --log-file=${VALGRIND_LOG}
)

set(CACHEGRIND_FLAGS
  --tool=cachegrind
  --cache-sim=yes
  --branch-sim=yes
  --dump-instr=yes
  --cachegrind-out-file=${CACHEGRIND_LOG}
)

set(CALLGRIND_FLAGS
  --tool=callgrind
  --callgrind-out-file=${CALLGRIND_LOG}
)

set(MASSIF_FLAGS
  --tool=massif
  --massif-out-file=${MASSIF_LOG}
)

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