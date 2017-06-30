#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <inttypes.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

#define MULTICAST_LAN "udpm://239.255.76.67:7667?ttl=64"

void usage(void)
{
    printf("usage: dogesay [-rvh] '[phrase]'\n");
    printf("    [--rate=num] [--voice=Alex]\n");
    printf("    [--daemon]\n");
    printf("    [--help]\n");
}

void say_some_help()
{
    char *phrase = ""
    "Hello, welcome to dogesay.  I will be your guide.  This program can run "
    "in a publishing mode, or a subscribing mode.  When run as a daemon, this "
    "program will listen for messages published by other dogesay processes and"
    " then say them based on the other parameters passed in like rate and "
    "voice.  Please consider the following command line options.  Dash dash "
    "rate will allow you to make me speak very slowly, or very quickly.  Dash "
    "dash voice will allow you to, among other things, change my gender. "
    "Finally, dash dash docs will relay this message, so that you understand "
    "how this program works.  I hope I've been helpful.  Next, please listen "
    "to my story.  Chapter 1.  The little doge that could.  One day, in the "
    "land of doges, there lived a little doge.  To be continued.";

    signal(SIGCHLD, SIG_IGN);
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execlp("/usr/bin/say", "say", "-r", "120", phrase, NULL);
    }
}

static void msg_handler(const lcm_recv_buf_t *rbuf, const char *channel,
        const doge_say_t *msg, void *user)
{
    char rate[64];
    snprintf(rate, 64, "%d", msg->rate);

    signal(SIGCHLD, SIG_IGN);
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execlp("/usr/bin/say", "say", "-v", msg->voice, "-r", rate,
                msg->phrase, NULL);
    }
}

void daemonize(void)
{
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        printf("dogesay daemon started [%d]  God help us all.\n", pid);
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

    lcm_t *lcm = lcm_create(NULL);
    if (!lcm) {
        exit(EXIT_FAILURE);
    }

    doge_say_t_subscribe(lcm, "DOGESAY", &msg_handler, NULL);

    while(1) {
        lcm_handle(lcm);
    }

    lcm_destroy(lcm);

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    doge_say_t dogesay = {
        .rate = 1,
        .voice = "Alex",
        .phrase = "",
    };

    int daemon, say_help;
    struct option longopts[] = {
        { "rate",     required_argument, NULL,      'r' },
        { "voice",    required_argument, NULL,      'v' },
        { "daemon",   no_argument,       &daemon,   1   },
        { "help",     no_argument,       &say_help, 1   },
        { NULL,       0,                 NULL,      0   }
    };

    char ch;
    while ((ch = getopt_long(argc, argv, "r:v:?", longopts, NULL)) != -1) {
        switch (ch) {
        case 0:
            if (daemon) {
                daemonize();
                exit(EXIT_SUCCESS);
            }
            if (say_help) {
                say_some_help();
                exit(EXIT_SUCCESS);
            }
            break;
        case 'r':
            dogesay.rate = strtol(optarg, NULL, 0);
            break;
        case 'v':
            dogesay.voice = optarg;
            break;
        default:
            usage();
            exit(EXIT_SUCCESS);
        }
    }
    argc -= optind;
    argv += optind;

    lcm_t *lcm = lcm_create(MULTICAST_LAN);
    if (!lcm) {
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    if (argc > 0) {
        strncpy(buf, argv[0], 1024);
        dogesay.phrase = buf;
        doge_say_t_publish(lcm, "DOGESAY", &dogesay);
    }
    else {
        while (fgets(buf, 1024, stdin) != NULL) {
            dogesay.phrase = buf;
            doge_say_t_publish(lcm, "DOGESAY", &dogesay);
        }
    }

    lcm_destroy(lcm);
    return 0;
}
