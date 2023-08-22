PROJECT_NAME = Project
OUTPUT_DIR = build
INCLUDE_DIR = include
SOURCE_DIR = src
C_SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SOURCE_DIR)/*.s)
ALL_SOURCES = $(C_SOURCES) $(ASM_SOURCES)
ALL_OBJS = $(ALL_SOURCES:$(SOURCE_DIR)/%=$(OUTPUT_DIR)/objs/%.o)

XC32_PREFIX = /opt/microchip/xc32/v4.30
GCC = $(XC32_PREFIX)/bin/xc32-gcc
OBJDUMP = $(XC32_PREFIX)/bin/xc32-objdump
BIN2HEX = $(XC32_PREFIX)/bin/xc32-bin2hex

PROCESSOR = 32MX795F512L
GCC_ASFLAGS = -Wa,--defsym=__MPLAB_BUILD=1,--gdwarf-2
GCC_LDFLAGS = -Wl,--defsym=__MPLAB_BUILD=1,--no-code-in-dinit,--no-dinit-in-serial-mem
CFLAGS = -std=c11 -fno-common -mprocessor=$(PROCESSOR) -I$(INCLUDE_DIR) -isystem $(XC32_PREFIX)/pic32mx/include -mdfp="/opt/microchip/mplabx/v5.30/packs/Microchip/PIC32MX_DFP/1.1.215"

DEBUG ?= 1

ifeq ($(DEBUG),0)
OPT_FLAGS = -O2 -DNDEBUG
else
OPT_FLAGS = -g
endif

all: compile_commands.json $(OUTPUT_DIR)/$(PROJECT_NAME).hex

$(OUTPUT_DIR):
	@mkdir -p $@

$(OUTPUT_DIR)/objs:
	@mkdir -p $@

$(OUTPUT_DIR)/objs/%.o: $(SOURCE_DIR)/% | $(OUTPUT_DIR)/objs Makefile
	$(GCC) $(CFLAGS) $(GCC_ASFLAGS) $(OPT_FLAGS) -c -o $@ $^

$(OUTPUT_DIR)/%.lst: $(OUTPUT_DIR)/objs/%.o | $(OUTPUT_DIR)/objs Makefile
	$(OBJDUMP) -drwC $< > $@

$(OUTPUT_DIR)/$(PROJECT_NAME).elf: $(ALL_OBJS) | $(OUTPUT_DIR)/objs Makefile
	$(GCC) $(CFLAGS) $(GCC_LDFLAGS) $(OPT_FLAGS) -o $@ $^

$(OUTPUT_DIR)/$(PROJECT_NAME).hex: $(OUTPUT_DIR)/$(PROJECT_NAME).elf | $(OUTPUT_DIR) Makefile
	$(BIN2HEX) $<

clean-objs:
	rm -rf $(OUTPUT_DIR)/objs

compile_commands.json: clean-objs | Makefile
	bear -- $(MAKE) $(ALL_OBJS)
	sed -i -E 's|\-mprocessor\=([A-Z0-9]+)|-D__\1__|g' $@
	sed -i -E 's|\"\-mdfp\=.*\"|\"-D__LANGUAGE_C__\"|g' $@

clean:
	rm -f compile_commands.json
	rm -rf $(OUTPUT_DIR)

.PHONY: clean-objs clean
