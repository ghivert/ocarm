# put your *.c source files here, make should handle the rest!
SRCS = floats.c      startup.c \
       array.c      freelist.c    stm32f3_discovery_accelerometer.c \
       backtrace.c  gc_ctrl.c     md5.c            stm32f3_discovery.c \
       callback.c   globroots.c   memory.c         stm32f3_discovery_gyroscope.c \
       caml         hash.c        meta.c           stm32f3xx_hal_msp.c \
       compact.c    instrtrace.c  minor_gc.c       stm32f3xx_it.c \
       compare.c    instruct.h    misc.c           str.c \
       intern.c     obj.c         sys.c \
       custom.c     interp.c      ocamlvm.ml       syscalls.c \
       debugger.c   interp.o      parsing.c        system_stm32f3xx.c \
       dynlink.c    ints.c        printexc.c       terminfo.c \
       exec.h       io.c          roots.c          unix.c \
       extern.c     lexing.c      signals_byt.c    weak.c \
       fail.c       main.c        signals.c \
       finalise.c   major_gc.c    stacks.c \
       fix_code.c   Makefile      startup_aux.c


# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=ocamlvm

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
STD_PERIPH_LIB=lib

# Location of the linker scripts
LDSCRIPT_INC=device/ldscripts

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=extra/stm32f3-openocd.cfg

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
GDB=arm-none-eabi-gdb
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size

CFLAGS  = -Wall -g -std=gnu99 -Os  
CFLAGS += -mlittle-endian -mcpu=cortex-m4  -march=armv7e-m -mthumb
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

###################################################

vpath %.a $(STD_PERIPH_LIB)

ROOT=$(shell pwd)

CFLAGS += -I inc 
CFLAGS += -I Components/Common
CFLAGS += -I $(STD_PERIPH_LIB) 
CFLAGS += -I $(STD_PERIPH_LIB)/cmsis/device/ST/STM32F3xx/Include
CFLAGS += -I $(STD_PERIPH_LIB)/cmsis/include 
CFLAGS += -I $(STD_PERIPH_LIB)/HAL/inc
# CFLAGS += -I $(STD_PERIPH_LIB)/STM32_USB-FS-Device_Driver/inc
CFLAGS += -I Components
CFLAGS += -I src
CFLAGS += -include $(STD_PERIPH_LIB)/stm32f3xx_hal_conf.h
CFLAGS += -D STM32F303xC

STARTUP = device/startup_stm32f303xc.s # add startup file to build

# need if you want to build with -DUSE_CMSIS 
#SRCS += stm32f0_discovery.c
#SRCS += stm32f0_discovery.c stm32f0xx_it.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

###################################################

.PHONY: all lib proj program debug clean reallyclean

all: lib proj

-include $(DEPS)

lib:
	$(MAKE) -C $(STD_PERIPH_LIB)
	$(MAKE) -C Components

proj: 	$(PROJ_NAME).elf

dirs:
	mkdir -p deps objs
	touch dirs

objs/%.o : src/%.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

$(PROJ_NAME).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(STARTUP) -L$(STD_PERIPH_LIB) -lhal -LComponents -lcomponents -L$(LDSCRIPT_INC) -TSTM32F303XC_FLASH.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

program: all
	openocd -f $(OPENOCD_BOARD_DIR)/stm32f3discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

debug: program
	$(GDB) -x extra/gdb_cmds $(PROJ_NAME).elf

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f objs/*.o
	rm -f deps/*.d
	rm -f dirs
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

reallyclean: clean
	$(MAKE) -C $(STD_PERIPH_LIB) clean
