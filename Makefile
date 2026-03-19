TARGET = EBOOT
SRCS = main.cpp

include $(PS3DEV)/ppu_rules

all: $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(PPU_CXX) $^ -o $@

pkg:
	make_package_npdrm $(TARGET).elf PARAM.SFO ICON0.PNG $(TARGET).pkg
