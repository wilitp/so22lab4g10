CC := gcc

# To eliminate debugging messages use -DNDEBUG
CFLAGS := -O0 -std=gnu11 -Wall -Werror -Wno-unused-parameter -Werror=vla -g \
	  -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=26 -D_GNU_SOURCE
CPPFLAGS := `pkg-config --cflags glib-2.0`
LDFLAGS :=`pkg-config --libs glib-2.0` -lfuse

export CC
export CFLAGS
export CPPFLAGS
export LDFLAGS


HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET := fat-fuse
VOLUME := ../resources/file.img
MOUNTPOINT := mountpoint

all: $(TARGET)

mount:
	mkdir -p $(MOUNTPOINT)
	./$(TARGET) $(VOLUME) $(MOUNTPOINT)

mount-dev:
	mkdir -p $(MOUNTPOINT)
	./$(TARGET) -f $(VOLUME) $(MOUNTPOINT)

unmount:
	umount $(MOUNTPOINT)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test-ht: hierarchy_tree.o
	make -C tests test_ht

test-ft:
	make -C tests test_ft

clean:
	rm -f $(TARGET) $(OBJECTS) tags cscope*
	make -C tests clean

.PHONY: clean
