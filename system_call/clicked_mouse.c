#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <time.h>

#define EVENT4 "/dev/input/event5"
#define EVENT6 "/dev/input/event6"

enum data { LEFT = 272, RIGHT = 273, CENTER = 274, POSITION_x = 0, POSITION_y = 1};

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
            case 'h': {
                printf("Please use option -c or -p");          
                break;
            }
            default:
                printf("Please use option -c or -p");          
                break;
        };
     
    if ( flag == 1)
        fd = open(EVENT6, O_RDONLY);
    else 
        fd = open(EVENT4, O_RDONLY);
    
    while (1) {
        int ret = read(fd, &a, sizeof(a));
        unsigned short code = a.code;
        unsigned short type = a.type;
        unsigned int value = a.value;
        switch(code) {
            case LEFT: { 
                if(value == 1)
                    printf("LEFT mouse is clicked\n");
                break;
            }
            case RIGHT: { 
                if(value == 1)
                    printf("RIGHT mouse is clicked\n");
                break;
            }
            case CENTER: { 
                if(value = 1)
                    printf("CENTER is clicked\n");
                break;
            }
            default:
              break;
        };

      // printf("Code: %d , Type: %d, Value: %d\n", code, type, value);
    }
    return 0;
}
