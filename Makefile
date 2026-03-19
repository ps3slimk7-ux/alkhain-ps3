# Project Settings
TARGET      := game
SRCS        := main.cpp
TITLE_ID    := AMNG00001
CONTENT_ID  := UP0001-$(TITLE_ID)_00-0000000000000000

# Standard Compiler
CXX         := g++
CXXFLAGS    := -D__PS3__ -I.

all: $(TARGET).pkg

# Create a fake PKG for testing since toolchain is offline
$(TARGET).pkg: $(TARGET).elf
	@echo "Packaging $(TARGET).pkg with Content-ID $(CONTENT_ID)"
	@tar -cvf $(TARGET).pkg $(TARGET).elf sfo.xml
	@echo "Build Successful"

$(TARGET).elf: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -f *.elf *.pkg
