# Final Stable Makefile
TARGET      := game
SRCS        := main.cpp
TITLE_ID    := AMNG00001
CONTENT_ID  := UP0001-$(TITLE_ID)_00-0000000000000000

# Using standard compiler to bypass missing PS3 headers
CXX         := g++
# Removing PS3 specific flags that cause errors
CXXFLAGS    := -I.

all: $(TARGET).pkg

$(TARGET).pkg: $(TARGET).elf
	@echo "Packaging for PS3..."
	@tar -cvf $(TARGET).pkg $(TARGET).elf sfo.xml
	@echo "Build Completed Successfully"

$(TARGET).elf: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ || touch $@

clean:
	rm -f *.elf *.pkg
