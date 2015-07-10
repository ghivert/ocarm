#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"

int Verbose = 0;
int Help    = 0;

char Directory[256] = "./";

char present[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

char *module[] = {
  "stm32_adc.cmo",       "stm32_can.cmo",  "stm32_cec.cmo",   "stm32_comp.cmo",
  "stm32_cortex.cmo",    "stm32_crc.cmo",  "stm32_dac.cmo",   "stm32_dma.cmo",
  "stm32_flash.cmo",     "stm32_sram.cmo", "stm32_nor.cmo",   "stm32_nand.cmo",
  "stm32_pccard.cmo",    "stm32_gpio.cmo", "stm32_hrtim.cmo", "stm32_i2c.cmo",
  "stm32_i2s.cmo",       "stm32_irda.cmo", "stm32_iwdg.cmo",  "stm32_opamp.cmo",
  "stm32_pwr.cmo",       "stm32_rcc.cmo",  "stm32_rtc.cmo",   "stm32_sdadc.cmo",
  "stm32_smartcard.cmo", "stm32_spi.cmo",  "stm32_tim.cmo",   "stm32_uart.cmo",
  "stm32_usart.cmo",     "stm32_wwdg.cmo", "stm32_ppp.cmo"
  };

char *flags[] = {
  "DFLAGS += -DHAL_ADC_MODULE_ENABLED\n",       "DFLAGS += -DHAL_CAN_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_CEC_MODULE_ENABLED\n",       "DFLAGS += -DHAL_COMP_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_CORTEX_MODULE_ENABLED\n",    "DFLAGS += -DHAL_CRC_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_DAC_MODULE_ENABLED\n",       "DFLAGS += -DHAL_DMA_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_FLASH_MODULE_ENABLED\n",     "DFLAGS += -DHAL_SRAM_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_NOR_MODULE_ENABLED\n",       "DFLAGS += -DHAL_NAND_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_PCCARD_MODULE_ENABLED\n",    "DFLAGS += -DHAL_GPIO_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_HRTIM_MODULE_ENABLED\n",     "DFLAGS += -DHAL_I2C_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_I2S_MODULE_ENABLED\n",       "DFLAGS += -DHAL_IRDA_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_IWDG_MODULE_ENABLED\n",      "DFLAGS += -DHAL_OPAMP_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_PWR_MODULE_ENABLED\n",       "DFLAGS += -DHAL_RCC_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_RTC_MODULE_ENABLED\n",       "DFLAGS += -DHAL_SDADC_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_SMARTCARD_MODULE_ENABLED\n", "DFLAGS += -DHAL_SPI_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_TIM_MODULE_ENABLED\n",       "DFLAGS += -DHAL_UART_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_USART_MODULE_ENABLED\n",     "DFLAGS += -DHAL_WWDG_MODULE_ENABLED\n",
  "DFLAGS += -DHAL_PPP_MODULE_ENABLED\n"
};

char *sources[] = {
  "\\\n       stm32f3xx_hal_adc.c          \\\n       stm32f3xx_hal_adc_ex.c       ",
  "\\\n       stm32f3xx_hal_can.c          ",
  "\\\n       stm32f3xx_hal_cec.c          ",
  "\\\n       stm32f3xx_hal_comp.c         ",
  "\\\n       stm32f3xx_hal_cortex.c       ",
  "\\\n       stm32f3xx_hal_crc.c          \\\n       stm32f3xx_hal_crc_ex.c       ",
  "\\\n       stm32f3xx_hal_dac.c          \\\n       stm32f3xx_hal_dac_ex.c       ",
  "\\\n       stm32f3xx_hal_dma.c          ",
  "\\\n       stm32f3xx_hal_flash.c        \\\n       stm32f3xx_hal_flash_ex.c     ",
  "\\\n       stm32f3xx_hal_sram.c         ",
  "\\\n       stm32f3xx_hal_nor.c          ",
  "\\\n       stm32f3xx_hal_nand.c         ",
  "\\\n       stm32f3xx_hal_pccard.c       ",
  "\\\n       stm32f3xx_hal_gpio.c         ",
  "\\\n       stm32f3xx_hal_hrtim.c        ",
  "\\\n       stm32f3xx_hal_i2c.c          \\\n       stm32f3xx_hal_i2c_ex.c       ",
  "\\\n       stm32f3xx_hal_i2s.c          \\\n       stm32f3xx_hal_i2s_ex.c       ",
  "\\\n       stm32f3xx_hal_irda.c         ",
  "\\\n       stm32f3xx_hal_iwdg.c         ",
  "\\\n       stm32f3xx_hal_opamp.c        \\\n       stm32f3xx_hal_opamp_ex.c     ",
  "\\\n       stm32f3xx_hal_pwr.c          \\\n       stm32f3xx_hal_pwr_ex.c       ",
  "\\\n       stm32f3xx_hal_rcc.c          \\\n       stm32f3xx_hal_rcc_ex.c       ",
  "\\\n       stm32f3xx_hal_rtc.c          \\\n       stm32f3xx_hal_rtc_ex.c       ",
  "\\\n       stm32f3xx_hal_sdadc.c        ",
  "\\\n       stm32f3xx_hal_smartcard.c    \\\n       stm32f3xx_hal_smartcard_ex.c ",
  "\\\n       stm32f3xx_hal_smbus.c        ",
  "\\\n       stm32f3xx_hal_spi.c          ",
  "\\\n       stm32f3xx_hal_tim.c          \\\n       stm32f3xx_hal_tim_ex.c       ",
  "\\\n       stm32f3xx_hal_uart.c         \\\n       stm32f3xx_hal_uart_ex.c      ",
  "\\\n       stm32f3xx_hal_usart.c        ",
  "\\\n       stm32f3xx_hal_wwdg.c         ",
  "\\\n       stm32f3xx_hal_ppp.c          "
};

void analyze_mod(char *name, FILE *macros, FILE *fsources) {
  for (int i = 0; i < 31; i++) {
    if ((strcmp(name, module[i]) == 0) && present[i] == 0) {
      fprintf(macros, "%s\n", flags[i]);
      fprintf(fsources, "%s", sources[i]);
      present[i] = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  char **files = malloc(sizeof (char *) * 100);
  int place = 0;

  // No inputs, exit.
  if (argc == 1) {
    fprintf(stdout, "No input files. ");
    fprintf(stdout, "Call \"modinc --help\" for help.\n");
    return EXIT_FAILURE;
  }

  // Scan args and activate or not flags.
  for (int i = 1; i < argc; i++)
  if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0))
    Help = 1;
  else if ((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0))
    Verbose = 1;
  else if ((strcmp(argv[i], "--file-numbers") == 0) || (strcmp(argv[i], "-f") == 0)) {
    if (realloc(files, sizeof (char *) * atoi(argv[++i])) == NULL) {
      fprintf(stderr, "Not enough memory.\n");
      return EXIT_FAILURE;
    }
  } else if ((strcmp(argv[i], "--output-directory") == 0) || (strcmp(argv[i], "-d") == 0)) {
    strncpy(Directory, argv[++i], 256);
  } else {
    if (place == 100) {
      fprintf(stderr, "Too much file to handle: tell how file you have ");
      fprintf(stderr, "with -f or --file-numbers.\n");
      return EXIT_FAILURE;
    } else files[place++] = argv[i];
  }

  // If help is activated, print help and exit.
  if (Help) {
    fprintf(stdout, "Utilisation : modinc [options] <inputs>\n");
    fprintf(stdout, "Read content of inputs (must be OCamlDep files), and analyze ");
    fprintf(stdout, "used modules for STM32.\n\n");

    fprintf(stdout, "-v, --verbose           Print commands done. Deactived by default.\n");
    fprintf(stdout, "-h, --help              Display help.\n");
    fprintf(stdout, "-f, --file-numbers      Indicate how much file to handle. Default is 100.\n");
    fprintf(stdout, "-d, --output-directory  Indicate the directory to place the files. Without trailing slash.\n");
    return EXIT_SUCCESS;
  }

  char file_path[1000];
  sprintf(file_path, "%s/Macros.conf", Directory);
  FILE *macros  = fopen(file_path, "w");
  sprintf(file_path, "%s/Sources.conf", Directory);
  FILE *sources = fopen(file_path, "w");
  fprintf(macros, "DFLAGS  = -DHAL_MODULE_ENABLED\n");
  fprintf(sources, "SRCS = stm32f3xx_hal.c              ");

  // Parse files to find modules.
  for (int i = 0; i < place; i++) {
    char buffer[1000];
    FILE *fd = fopen(files[i], "r");

    while (fgets(buffer, 1000, fd)) {
      strar words = strsplit(buffer, ' ');
      for (size_t i = 0; i < size_strar(words); i++) {
        analyze_mod(words[i], macros, sources);
      }
      free_strar(words);
    }
    fclose(fd);
  }

  fclose(macros);
  fclose(sources);

  free(files);
  return 0;
}
