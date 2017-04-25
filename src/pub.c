#include <stdio.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create(NULL);
    if (!lcm) {
        return 1;
    }

    doge_say_t dogesay = {
        .rate = 1,
        .voice = "Alex",
        .phrase = "yeah buddy",
    };

    doge_say_t_publish(lcm, "DOGESAY", &dogesay);

    lcm_destroy(lcm);
    return 0;
}
