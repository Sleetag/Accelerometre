UCLINUX_PATH=~/uClinux
UCLIBC_DIR=$(UCLINUX_PATH)/lib/uClibc
INCLUDEDIR_DIST=$(UCLINUX_PATH)/include
INCLUDEDIR_UCLINUX=$(UCLINUX_PATH)/linux-2.6.x/include
INCLUDEDIR_UCLIBC=$(UCLINUX_PATH)/lib/uClibc/include

ARM_LINUX_PATH=/usr/local/arm-linux
ARM_LINUX_INCLUDE=$(ARM_LINUX_PATH)/include
ARM_LINUX_LIB=$(ARM_LINUX_PATH)/lib

ARM_ELF_PATH=/usr/local/arm-elf
ARM_ELF_INCLUDE=$(ARM_ELF_PATH)/include
ARM_ELF_LIB=$(ARM_ELF_PATH)/lib
#RUNTIME=$(UCLIBC_DIR)/lib/crt0.o $(UCLIBC_DIR)/lib/crti.o $(UCLIBC_DIR)/lib/crtn.o

CC=arm-elf-gcc
#CFLAGS = -Wl -g -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -fpic -msoft-float -fno-builtin -nostartfiles -I$(ARM_LINUX_INCLUDE) -I$(INCLUDEDIR_DIST) -I$(INCLUDEDIR_UCLINUX) -I$(INCLUDEDIR_UCLIBC) -elf2flt -I.
#LDFLAGS = -L$(ARM_LINUX_LIB) -lm -L$(UCLIBC_DIR)/. -L$(UCLIBC_DIR)/lib 

#CFLAGS = -g -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D__PIC__ -fpic -msoft-float -fno-builtin -nostartfiles -I$(ARM_ELF_INCLUDE) -I.
#LDFLAGS = -Wl, --fatal-warnings -Wl, -elf2flt -msoft-float


CC=arm-elf-gcc
CFLAGS= -Wall -W 
LDFLAGS= -Wl, -elf2flt -I$(ARM_LINUX_INCLUDE)
LDLIBS = -L$(ARM_ELF_LIB) -lm -L$(UCLIBC_DIR)/. -L$(UCLIBC_DIR)/lib 


SRC = fb_Graphic.c hal.c
OBJS = fb_Graphic.o hal.o
EXEC = Hello



OBJ=$(SRC:%.c=%.o)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS) $(LDLIBS) 


clean:
	rm -f $(EXEC)  *.el* *.gdb *.o *~ romfs_5.img vmlinux.bin
