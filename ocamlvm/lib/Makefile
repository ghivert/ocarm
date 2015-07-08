CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c HAL/src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -march=armv7e-m
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32f3xx_hal_conf.h 
CFLAGS += -Icmsis/include 
CFLAGS += -Icmsis/device/ST/STM32F3xx/Include 
CFLAGS += -IHAL/inc
CFLAGS += #-ISTM32_USB-FS-Device_Driver/inc
CFLAGS += -I.
CFLAGS += -DSTM32F303xC

SRCS = stm32f3xx_hal_adc.c      stm32f3xx_hal_i2c_ex.c       stm32f3xx_hal_rcc_ex.c       \
       stm32f3xx_hal_adc_ex.c   stm32f3xx_hal_i2s.c          stm32f3xx_hal_rtc.c          \
       stm32f3xx_hal.c          stm32f3xx_hal_i2s_ex.c       stm32f3xx_hal_rtc_ex.c       \
       stm32f3xx_hal_can.c      stm32f3xx_hal_irda.c         stm32f3xx_hal_sdadc.c        \
       stm32f3xx_hal_cec.c      stm32f3xx_hal_iwdg.c         stm32f3xx_hal_smartcard.c    \
       stm32f3xx_hal_comp.c     stm32f3xx_hal_msp_template.c stm32f3xx_hal_smartcard_ex.c \
       stm32f3xx_hal_cortex.c   stm32f3xx_hal_nand.c         stm32f3xx_hal_smbus.c        \
       stm32f3xx_hal_crc.c      stm32f3xx_hal_nor.c          stm32f3xx_hal_spi.c          \
       stm32f3xx_hal_crc_ex.c   stm32f3xx_hal_opamp.c        stm32f3xx_hal_sram.c         \
       stm32f3xx_hal_dac.c      stm32f3xx_hal_opamp_ex.c     stm32f3xx_hal_tim.c          \
       stm32f3xx_hal_dac_ex.c   stm32f3xx_hal_pccard.c       stm32f3xx_hal_tim_ex.c       \
       stm32f3xx_hal_dma.c      stm32f3xx_hal_pcd.c          stm32f3xx_hal_tsc.c          \
       stm32f3xx_hal_flash.c    stm32f3xx_hal_pcd_ex.c       stm32f3xx_hal_uart.c         \
       stm32f3xx_hal_flash_ex.c stm32f3xx_hal_ppp.c          stm32f3xx_hal_uart_ex.c      \
       stm32f3xx_hal_gpio.c     stm32f3xx_hal_pwr.c          stm32f3xx_hal_usart.c        \
       stm32f3xx_hal_hrtim.c    stm32f3xx_hal_pwr_ex.c       stm32f3xx_hal_wwdg.c         \
       stm32f3xx_hal_i2c.c      stm32f3xx_hal_rcc.c          stm32f3xx_ll_fmc.c

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
DEPS = $(addprefix deps/,$(SRCS:.c=.d))

.PHONY: all clean

all: libhal.a

-include $(DEPS)

dirs:
	mkdir -p deps objs
	touch dirs

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

libhal.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f dirs $(OBJS) $(DEPS) libhal.a

