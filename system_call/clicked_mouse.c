#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <time.h>

#define EVENT4 "/dev/input/event4"
#define EVENT6 "/dev/input/event6"

int main(int argc, char *argv[]) {
    int fd;
    struct input_event a;
    printf("lengt %ld.\n",sizeof(a));
    int c;
    int flag = 0;
    while ((c = getopt (argc, argv, "hcp")) != -1) 
        switch(c) {
            case 'c': {
                fd = open(EVENT4, O_RDONLY);
                break;
            }
            case 'p': {
                flag = 1;
                fd = open(EVENT6, O_RDONLY);
                break;
            }
            default:
                break;
        };
    /* 
    if ( flag == 1)
        fd = open(EVENT6, O_RDONLY);
    else 
        fd = open(EVENT4, O_RDONLY);
    */

    while (1) {
        int ret = read(fd, &a, sizeof(a));
        unsigned short code = a.code;
        unsigned short type = a.type;
        unsigned int value = a.value;
        printf("Code: %d , Type: %d, Value: %d\n", code, type, value);
    }
    return 0;
}
