NAME?=switcher
MCU?=atmega168p
PROG=${NAME}-${MCU}
WRITER=avrisp2
CFLAGS=-Os -mmcu=${MCU} -Wall

SRCS=main.c
OBJS=${SRCS:%.c=%.o}

DC=docker-compose run --rm builder
TOOLDIR=/usr/local/avr
CC=${DC} ${TOOLDIR}/bin/avr-gcc
OBJDUMP=${DC} ${TOOLDIR}/bin/avr-objdump
OBJCOPY=${DC} ${TOOLDIR}/bin/avr-objcopy

.SUFFIXES= .c .o .S .s

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

.c.s:
	${CC} ${CFLAGS} -S -o $@ $<

.S.o:
	${CC} ${CFLAGS} -c -o $@ $<

.s.o:
	${CC} ${CFLAGS} -c -o $@ $<

all: ${PROG}.elf ${PROG}.hex

${PROG}.elf: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS}
	${OBJDUMP} -d $@ > ${PROG}.dump

${PROG}.hex: ${PROG}.elf
	${OBJCOPY} -I elf32-avr -O ihex ${PROG}.elf ${PROG}.hex

clean:
	rm -f *.s *.o *.dump *.hex *.elf

edit:
	avrdude -P usb -c ${WRITER} -p ${MCU} -t

write:
	avrdude -P usb -c ${WRITER} -p ${MCU} -U flash:w:${PROG}.hex
