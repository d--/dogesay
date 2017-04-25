#include <stdio.h>
#include <unistd.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

#define MULTICAST_LAN "udpm://239.255.76.67:7667?ttl=1"

int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create(MULTICAST_LAN);
    if (!lcm) {
        return 1;
    }

    doge_say_t dogesay = {
        .rate = 1,
        .voice = "Alex",
        .phrase = "yeah buddy",
    };

    char buf[1024];
    fgets(buf, 1024, stdin);
    while (1) {
        dogesay.phrase = buf;
        doge_say_t_publish(lcm, "DOGESAY", &dogesay);
        fgets(buf, 1024, stdin);
    }

    lcm_destroy(lcm);
    return 0;
}
