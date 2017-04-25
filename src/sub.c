#include <stdio.h>
#include <unistd.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

static void handler(const lcm_recv_buf_t *rbuf, const char *channel,
        const doge_say_t *msg, void *user)
{
    char rate[64];
    snprintf(rate, 64, "%d", msg->rate);

    pid_t pid = fork();
    if (pid == 0) {
        execlp("/usr/bin/say",
                "say",
                "-v",
                msg->voice,
                "-r",
                rate,
                msg->phrase,
                NULL);
    }
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
