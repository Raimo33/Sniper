#TODO fare un bel markdown dove spiego le ottimizzazioni fatte
#TODO benchmark e grafico che misura il miglioramento dopo ogni ottimizzazione? (oppure semplicemente differenza tra prima e dopo)

object pool
epoll
single threaded, single process
non-blocking sockets
Edge Triggered events
async logging with events
log ring buffer
jemalloc
non-blocking DNS
Kernel Bypass Networking
high-resolution async timers (timerfd_create())
stack allocation
in-memory order books ((binary heap))
CPU caching
SIMD
memory alignment
memory pools
wolfSSL (TLS)
TCP_NODELAY (disable Nagle's algorithm)
everything O(1)
inline functions
prefetching
link-time optimization
vectorized writes
vectorized reads
memory fencing
zero copy
NUMA
software pipelining
memory mapped files
inline assemlby __asm__ volatile

gcc extensions:
aggressive Ofast compilation
-march=native
conditional gotos (jump tables)
fixed-point arithmetic for prices
__attribute__((cold)) e __attribute__((hot))
__attribute__((aligned(16)))
__attribute__((fallthrough))
__attribute__((noinline)) e __attribute__((always_inline)) e __attribute__((flatten))
__attribute__((packed))
__attribute__((pure)) e __attribute__((const))
__attribute__((malloc))
__attribute__((unused))
__attribute__((unreachable))
__attribute__visibility__((hidden))
__builtin_expect per conditionals
__builtin_prefetch
restrict keyword

#pragmas: #TODO https://gcc.gnu.org/onlinedocs/gcc/Pragmas.html
- ivdep