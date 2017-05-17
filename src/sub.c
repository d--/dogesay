#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

static void handler(const lcm_recv_buf_t *rbuf, const char *channel,
        const doge_say_t *msg, void *user)
{
    char rate[64];
    snprintf(rate, 64, "%d", msg->rate);

    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execlp("/usr/bin/osascript", "osascript", "-e", "set Volume 3", NULL);
    }

    pid = fork();
    if (pid == 0) {
        execlp("/usr/bin/say", "say", "-v", msg->voice, "-r", rate,
                msg->phrase, NULL);
    }
}

int main(int argc, char **argv)
{
    /*
     * Daemonize
     */
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    /* open logs here */

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    /*
     * End daemonization
     */

    lcm_t *lcm = lcm_create(NULL);
    if (!lcm) {
        return 1;
    }

    doge_say_t_subscribe(lcm, "DOGESAY", &handler, NULL);

    while(1) {
        lcm_handle(lcm);
    }

    lcm_destroy(lcm);

    exit(EXIT_SUCCESS);
}
