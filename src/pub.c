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

    char *str;
    char buf[1024];
    while ((str = fgets(buf, 1024, stdin)) != NULL) {
        dogesay.phrase = str;
        doge_say_t_publish(lcm, "DOGESAY", &dogesay);
    }

    lcm_destroy(lcm);
    return 0;
}
