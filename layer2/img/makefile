TARGETS=img_logo.h t_floor1.h t_floor2.h s_bug1a.h s_bug1b.h s_pow1a.h s_pow1b.h s_pow1c.h

all: $(TARGETS)

%.h: %.png
	../tools/img_to_tft_h $^ $(shell basename $< .png) 

clean:
	rm -f $(TARGETS)

