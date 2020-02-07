#include <string.h>
#include <sys/mman.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#define BuffSize 4096
int main (int argc,char *argv[]) {
    int fd = open(argv[1],O_RDWR);
    std::cout << fd << std::endl;
    // lseek(fd,4*1024*1024,SEEK_END);
    // write(fd,"c",1);
    // fd = open(argv[1],O_RDONLY);
    struct stat st;
    if(stat(argv[1],&st) < 0)
        exit(0);
    std::cout << "filesize: " << st.st_size << std::endl;
    char *tmpaddr = NULL,*addr = NULL;
    ssize_t length = st.st_size;
    int offset = 0;
    addr = tmpaddr = (char*)mmap(NULL,BuffSize,PROT_READ,MAP_PRIVATE,fd,offset);
    // assert(webResponse::fileAddr != MAP_FAILED);
     length -= BuffSize;
     int count = 0;
    while(length > 0) {
        tmpaddr += BuffSize;
        offset += BuffSize;
       
        tmpaddr = (char*)mmap(NULL,BuffSize,PROT_READ,MAP_PRIVATE,fd,offset);
        assert(tmpaddr != MAP_FAILED);
        count++;
        if(length > 0 && length < BuffSize) {
            tmpaddr = (char*)mmap(NULL,BuffSize,PROT_READ,MAP_PRIVATE,fd,offset);
            count++;            
        }

        length -= BuffSize;
        std::cout << length << std::endl;
    }
    ::close(fd);
    std::cout << addr << std::endl;
    while(count > 0) {
        munmap(tmpaddr,BuffSize);
        count--;
        tmpaddr -= BuffSize;
    }
    tmpaddr = NULL;
    return 0;
}