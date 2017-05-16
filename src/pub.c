#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <inttypes.h>
#include <lcm/lcm.h>

#include "doge_say_t.h"

#define MULTICAST_LAN "udpm://239.255.76.67:7667?ttl=1"

void usage(void)
{
    printf("usage: pub [-rvh] '[phrase]'\n");
    printf("    [--rate=num] [--voice=Alex]\n");
}

int main(int argc, char **argv)
{
    lcm_t *lcm = lcm_create(MULTICAST_LAN);
    if (!lcm) {
        return 1;
    }

    doge_say_t dogesay = {
        .rate = 1,
        .voice = "Alex",
        .phrase = "yeah",
    };

    struct option longopts[] = {
        { "rate",   required_argument, NULL, 'r' },
        { "voice",  required_argument, NULL, 'v' },
        { "help",   no_argument,       NULL, 'h' },
        { NULL,     0,                 NULL, 0 }
    };

    char ch;
    while ((ch = getopt_long(argc, argv, "r:v:h", longopts, NULL)) != -1) {
        switch (ch) {
        case 'r':
            dogesay.rate = strtol(optarg, NULL, 0);
            break;
        case 'v':
            dogesay.voice = optarg;
            break;
        default:
            usage();
            return 0;
        }
    }
    argc -= optind;
    argv += optind;

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
