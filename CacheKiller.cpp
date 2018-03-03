#include <stdio.h>
#include <io.h>
#include <fcntl.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{   
    int count = 1;
    if(argc>1)
    {
        int c = atoi(argv[1]);
        count = c > 0 ? c : count;
    }

    int fh;
    unsigned sz = 1000000000;
    char *p = new char[sz];
    int k;

    printf("Killing disk cache...\n");
    unlink("killer");
    fh = open("killer", O_RDWR | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);
    for(int i = 0; i < count; i++)
    {
        write(fh, p, sz);
    }
    close(fh);

    fh=open("killer",O_RDWR | O_BINARY, S_IREAD | S_IWRITE);
    for(int i = 0; i < count; i++)
    {
        k=read(fh,p,sz);
    }    
    close(fh);
    unlink("killer");
    delete [] p;
    printf("Done.\n");
    return 0;
}