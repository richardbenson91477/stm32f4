TARGETS=to_565 uint16_h colors565.h
CFLAGS+=
LDLIBS+=

all: $(TARGETS)

colors565.h: gen_colors_565.py rgb.txt
	./gen_colors_565.py

clean:
	rm -f $(TARGETS)

