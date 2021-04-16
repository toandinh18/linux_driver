#include <stdio.h>

int main (){
    int a;
    int b = 100;
    a = fork();
   // if (a == 0) {
   //     printf("This is child process: %d\n",getpid());
   //     //b = b + 10;
   //     printf("b = %d\n",b);
   // }
   // else if (a > 0){
   //     printf("This is parent process: %d\n", getpid());
   //     b = b + 100;
   //     printf("b = %d\n",b);
   // }
    fork();
    fork();
    printf("aa\n"); 
    return 0;
}
