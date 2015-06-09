CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

########################################

STD_LIB = ../lib

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -march=armv7e-m
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -I $(STD_LIB)/cmsis/include 
CFLAGS += -I $(STD_LIB)/cmsis/device/ST/STM32F3xx/Include 
CFLAGS += -I $(STD_LIB)/HAL/inc
CFLAGS += -I Common/
CFLAGS += -include $(STD_LIB)/stm32f3xx_hal_conf.h
# CFLAGS += -I STM32_USB-FS-Device_Driver/inc
# CFLAGS += -I .
CFLAGS += -D STM32F303xC

VPATH  = cs42l52    cs43l22    hx8347d       hx8347g        
VPATH += ili9328    l3gd20     lsm303dlhc    spfd5408
VPATH += st7735     stlm75     stts751       hx42l52

SRCS = cs42l52.c         cs43l22.c     hx8347d.c   \
       hx8347g.c         ili9328.c     l3gd20.c    \
       lsm303dlhc.c      spfd5408.c    st7735.c    \
       stlm75.c          stts751.c 

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

.PHONY: all clean

all: libcomponents.a

-include $(DEPS)

dirs:
	mkdir -p deps objs
	touch dirs

objs/%.o: %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

libcomponents.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f dirs $(OBJS) $(DEPS) libcomponents.a
