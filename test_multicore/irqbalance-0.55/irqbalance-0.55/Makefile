MAKEFLAGS += -s
BOLD = tput bold
NORMAL = tput sgr0

THIS_SETUP = /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/setup.txt
SOURCEPATH = /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/
OBJECTPATH = /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/z_lib/z_obj/homesdrWORKINGtest_multicoreirqbalance-0.55irqbalance-0.55setup.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/
LIBRARYPATH = /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/

FLAGS = -Wswitch-default -ggdb -DDEBUG -g -Os -D_FORTIFY_SOURCE=2 -Wall -W `pkg-config --cflags glib-2.0` 
INCLUDES = -c -I$(SOURCEPATH)
LINKS =  -Wl,--start-group -lm -lpthread -Wl,--end-group

CXX = ccache gcc $(MACH) $(PARAM)
THIS_LIB = $(LIBRARYPATH)libhomesdrWORKINGtest_multicoreirqbalance-0.55irqbalance-0.55setup.txt.a



OBJECT = $(OBJECTPATH)bitmap.o \
         $(OBJECTPATH)cputree.o \
         $(OBJECTPATH)procinterrupts.o \
         $(OBJECTPATH)irqlist.o \
         $(OBJECTPATH)placement.o \
         $(OBJECTPATH)activate.o \
         $(OBJECTPATH)network.o \
         $(OBJECTPATH)powermode.o \
         $(OBJECTPATH)numa.o \
         $(OBJECTPATH)classify.o \

SOURCE = $(SOURCEPATH)bitmap.c \
         $(SOURCEPATH)cputree.c \
         $(SOURCEPATH)procinterrupts.c \
         $(SOURCEPATH)irqlist.c \
         $(SOURCEPATH)placement.c \
         $(SOURCEPATH)activate.c \
         $(SOURCEPATH)network.c \
         $(SOURCEPATH)powermode.c \
         $(SOURCEPATH)numa.c \
         $(SOURCEPATH)classify.c \

PROG0   = irqbalance
PROG0_l = $(OBJECTPATH)$(PROG0)
OBJECT0 = $(OBJECTPATH)$(PROG0).o
SOURCE0 = $(OBJECTPATH)$(PROG0).c

all: checkdir /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/Makefile $(THIS_LIB) $(PROG0_l) progs cleanup

$(OBJECTPATH)%.d: /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/setup.txt
	rm -f $(OBJECTPATH)$(*F).d

-include $(PROG0_l).d
-include $(subst $(SOURCEPATH), $(OBJECTPATH), $(SOURCE:.c=.d))
$(OBJECTPATH)%.o: $(SOURCEPATH)%.c /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/setup.txt
	$(CXX) -MF $(OBJECTPATH)$(*F).d -MT $(OBJECTPATH)$(*F).o -MM $(FLAGS) $(INCLUDES) $<
	echo "Compiling $<"
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(THIS_LIB): $(OBJECT) /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/setup.txt
	echo "Archiving library $(THIS_LIB)"
	rm -f $(THIS_LIB); \
	ar rc $(THIS_LIB) $(OBJECT); \
	ranlib $(THIS_LIB)

.PHONY: progs
progs:
	rm -f $(PROG0)
	ln -s $(PROG0_l)

$(PROG0_l): $(OBJECT) $(OBJECT0) /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/Makefile
	echo "Building $(SOURCEPATH)$(PROG0)"
	$(CXX) -g -O2 -D_FORTIFY_SOURCE=2 -Wall  `pkg-config --libs glib-2.0` -o $(PROG0_l) $(OBJECT) $(OBJECT0) $(LINKS)

.PHONY: clean
clean:
	echo "Cleaning everything in /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55"
	rm -f $(OBJECTPATH)*
	rm -f $(LIBRARYPATH)*
	rm -f $(PROG0)

.PHONY: clean_all
clean_all: clean

.PHONY: cleanup
cleanup:
	touch $(THIS_LIB)

.PHONY: checkdir
checkdir:
	if [ ! -d $(OBJECTPATH) ]; then mkdir -p $(OBJECTPATH); fi
	if [ ! -d $(LIBRARYPATH) ]; then mkdir -p $(LIBRARYPATH); fi

/home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/Makefile: /home/sdr/WORKING/test_multicore/irqbalance-0.55/irqbalance-0.55/setup.txt
	echo "setup or fpga file list has changed: call domakefile before compiling."
	exit 1

