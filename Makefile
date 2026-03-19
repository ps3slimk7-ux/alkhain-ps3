TITLE       := Among PS3
APPID       := AMNG00001
CONTENTID   := UP0001-$(APPID)_00-0000000000000000

include $(PS3DEV)/ppu_rules

TARGET      := game
LIBS        := -lgcm_sys -lio -lsysutil -lrt -llv2 -lsysmodule -lm

all: $(TARGET).pkg

PARAM.SFO: sfo.xml
	$(SFO) -f $< $@

$(TARGET).elf: main.cpp
	$(PPU_CXX) -O2 -Wall -mcpu=cell main.cpp -o $@ $(LIBS)

$(TARGET).pkg: $(TARGET).elf PARAM.SFO
	$(PKG) --contentid $(CONTENTID) $(TARGET).elf PARAM.SFO ICON0.PNG $@

clean:
	rm -f *.o *.elf *.pkg PARAM.SFO
