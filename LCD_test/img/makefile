TARGETS=img1.h
all: $(TARGETS)

%.h: %.png
	../tools/img_to_tft_h $^ $(shell basename $< .png) 

clean:
	rm -f $(TARGETS)

