#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[]){
    int p[2],p1[2];

    pipe(p);
    pipe(p1);

    char byte='p';
    char byte2;
    // argv[0] = "echo";
    if (fork()== 0) {
        close(0);
        // dup(p1[1]);
        close(p[1]);
        close(p1[0]);

        read(p[0],&byte2,1);
        close(p[0]);

        if (byte2==byte){
            
            int pid = getpid();

            printf("%d: received ping\n",pid);
        
            write(p1[1],&byte,1);

            close(p1[1]);
        }
        else {
            printf("wrong byte on ping!");
        }

    }
    else {
        close(0);
        close(p[0]);
        close(p1[1]);

        write(p[1],&byte,1);

        close(p[1]);

        read(p1[0],&byte2,1);
        
        close(p1[0]);

        if (byte2==byte){
            int pid = getpid();
            printf("%d: received pong\n",pid);
        }
        else{
            printf("Wrong byte on pong!");
        }
        
    }

    exit(0);
}