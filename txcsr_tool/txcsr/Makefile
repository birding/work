#simple make file

CC=${CROSS_COMPILE}gcc

CFLAGS= -I . \
	-I ./bdk \
	-D_FILE_OFFSET_BITS=64 \
	-D_LARGEFILE64_SOURCE \
	-Wall -std=gnu99

OBJ=	txcsr.o \
	helper.o \
	bdk/bdk-csrs.o \
	bdk/bdk-csr-db.o

all: txcsr

txcsr: ${OBJ}
	${CC} -o txcsr ${OBJ} ${CFLAGS}

clean:
	rm -rf *.o txcsr bdk/*.o
