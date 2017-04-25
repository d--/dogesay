#include <stdio.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

static void handler(const lcm_recv_buf_t *rbuf, const char *channel,
        const doge_say_t *msg, void *user)
{
    printf("message received!\n");
    printf("rate: %lld\n", msg->rate);
    printf("voice: %s\n", msg->voice);
    printf("phrase: %s\n", msg->phrase);
}

int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create(NULL);
    if (!lcm) {
        return 1;
    }

    doge_say_t_subscribe(lcm, "DOGESAY", &handler, NULL);

    while(1) {
        lcm_handle(lcm);
    }

    lcm_destroy(lcm);
    return 0;
}
