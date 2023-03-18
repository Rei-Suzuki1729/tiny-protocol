#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>


#include "util.h"
#include "net.h"
#include "ip.h"

#include "driver/loopback.h"

#include "test.h"

static volatile sig_atomic_t terminate;

static void
on_signal(int s)
{
    (void)s;
    terminate = 1;
}

static int
setup(void)
{
    struct net_device *dev;
    struct ip_iface *iface;

    signal(SIGINT, on_signal);
    if (net_init() == -1) {
        errorf("net_init() failure");
        return -1;
    }
}

static void
cleanup(void)
{
    net_shutdown();
}

int
main(int argc, char *argv[])
{
    ip_addr_t src, dst;
    size_t offset = IP_HDR_SIZE_MIN;

    if (setup() == -1) {
        errorf("setup() failure");
        return -1;
    }
    ip_addr_pton(LOOPBACK_IP_ADDR, &src);
    dst = src;
while (!terminate) {
    if (ip_output(1, test_data + offset, sizeof(test_data) - offset, src, dst) == -1) {
        errorf("ip_output() failure");
        break;
    }
    sleep(1);
}
cleanup();
return 0;

}
