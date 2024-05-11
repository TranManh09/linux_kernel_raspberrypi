#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define DEVICE_FILE "/dev/my_i2c_driver"

int main() {
    int file_desc;
    char value;
	char * data;
    // Mở thiết bị
    file_desc = open(DEVICE_FILE, O_RDWR);
    if (file_desc < 0) {
        printf("Không thể mở thiết bị.\n");
        return -1;
    }
	
    
    while(1){
    	printf("enter string: ");
	    fgets(data, 100, stdin);
        //scanf("%s", data);
        write(file_desc, data, strlen(data));
        printf("\n");
        printf("%s\n",data);
        printf("%d", sizeof(*data));
    }

    // Đóng thiết bị
    close(file_desc);

    return 0;
}

