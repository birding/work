#simple make file

CC=${CROSS_COMPILE}gcc

CFLAGS= -I ./txcsr \
	-I ./txcsr/bdk \
	-D_FILE_OFFSET_BITS=64 \
	-D_LARGEFILE64_SOURCE \
	-D_X86_HOST_ \
	-Wall -std=gnu99

OBJ= txcsr/txcsr.o \
	txcsr/helper.o \
	txcsr/bdk/bdk-csrs.o \
	txcsr/bdk/bdk-csr-db.o

all: txcsr_tool

txcsr_tool: ${OBJ}
	${CC} -o txcsr_x86 ${OBJ} ${CFLAGS}

clean:
	rm -rf *.o txcsr_x86 txcsr/bdk/*.o txcsr/*.o
