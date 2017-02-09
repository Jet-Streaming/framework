#ifdef CONFIG_FASTPATH_DPDK
#include <fastpath/dpdk/dpdkpacket.h>

// TODO #elif defined(CONFIG_SLOWPATH)

#else
// slow path
#include "ospacket.h"
#endif //CONFIG_FASTPATH_DPDK