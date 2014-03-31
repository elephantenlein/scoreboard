#--------------------------------------------------
# C.O. says, this is a Makefile
#--------------------------------------------------
MCU=atmega64
PROJ=scoreboard
PROGRAMMER = avrisp2
#PROGRAMMER = jtag2isp

SECTIONS = 

# files
#--------------------------------------------------
SOURCES = main ste2007 symbols display keyboard
BUILDDIR = build
BINDIR = bin
FUSEDIR = fuse

# programs
#--------------------------------------------------
CC = avr-gcc
PGM = avrdude
OBJCOPY = avr-objcopy

# flags
#--------------------------------------------------
COMMON = -mmcu=$(MCU)

CFLAGS = $(COMMON) -mmcu=$(MCU) -Wall -gdwarf-2 -DF_CPU=8000000UL -Os -std=gnu99
LDFLAGS = $(COMMON) $(SECTIONS)
PGMFLAGS = -B 16 -P usb -p $(MCU) -c $(PROGRAMMER) -u -V

HEX_FLASH_FLAGS = -R .eeprom \
		  -R .fuse \
		  -R .lock \
		  -R .signature \
		  -O ihex

HEX_EEPROM_FLAGS = -j .eeprom \
		   -O ihex

# settings
#--------------------------------------------------
.PHONY: burn burn-ee burn-fuse burn-lock dump dump-ee dump-fuse clean cleanup

# rules
#--------------------------------------------------
all: $(BINDIR)/$(addsuffix .hex, $(PROJ)) \
     $(BINDIR)/$(addsuffix -ee.hex, $(PROJ))

$(BINDIR)/$(addsuffix .elf, $(PROJ)): $(addprefix $(BUILDDIR)/, $(addsuffix .o,$(SOURCES)))
	@echo     [LD] $(notdir $^)
	@$(CC) $(LDFLAGS) $^ -o $(BINDIR)/$(addsuffix .elf, $(PROJ))

$(BINDIR)/$(addsuffix .hex, $(PROJ)): $(BINDIR)/$(addsuffix .elf, $(PROJ))
	@echo     [HEX] $(notdir $@)
	@$(OBJCOPY) $(HEX_FLASH_FLAGS) $< $@

$(BINDIR)/$(addsuffix -ee.hex, $(PROJ)): $(BINDIR)/$(addsuffix .elf, $(PROJ))
	@echo     [EE-HEX] $(notdir $@)
	@$(OBJCOPY) $(HEX_EEPROM_FLAGS) $< $@

# burning rules
#--------------------------------------------------
burn: $(BINDIR)/$(addsuffix .hex, $(PROJ))
	$(PGM) $(PGMFLAGS) -U flash:w:$<:i

burn-ee: $(BINDIR)/$(addsuffix -ee.hex, $(PROJ))
	$(PGM) $(PGMFLAGS) -U eeprom:w:$<:i

burn-fuse:
	$(PGM) $(PGMFLAGS) -U lfuse:w:$(FUSEDIR)/lfuse.hex:i -U hfuse:w:$(FUSEDIR)/hfuse.hex:i -U efuse:w:$(FUSEDIR)/efuse.hex:i

burn-lock:
	$(PGM) $(PGMFLAGS) -U lock:w:$(FUSEDIR)/lock.hex:i

# dumping rules
#--------------------------------------------------
dump:
	$(PGM) $(PGMFLAGS) -U flash:r:$(BINDIR)/flash.hex:i

dump-ee:
	$(PGM) $(PGMFLAGS) -U eeprom:r:$(BINDIR)/eeprom.hex:i

dump-fuse:
	$(PGM) $(PGMFLAGS) -U lfuse:r:$(FUSEDIR)/dlfuse.hex:i -U hfuse:r:$(FUSEDIR)/dhfuse.hex:i -U efuse:r:$(FUSEDIR)/defuse.hex:i

# sanitary rules
#--------------------------------------------------
clean:
	rm -f $(BUILDDIR)/*

cleanup: clean
	rm -f $(BINDIR)/*

# pattern rules
#--------------------------------------------------
$(BUILDDIR)/%.o: %.c
	@echo     [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

