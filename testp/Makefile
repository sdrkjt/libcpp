MAKEFLAGS += -s
BOLD = tput bold
NORMAL = tput sgr0

THIS_SETUP = /home/sdr/WORKING/testp/setup.txt
SOURCEPATH = /home/sdr/WORKING/testp/
OBJECTPATH = /home/sdr/WORKING/testp/z_lib/z_obj/homesdrWORKINGtestpsetup.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/
LIBRARYPATH = /home/sdr/WORKING/testp/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/

FLAGS = -Wswitch-default -ggdb -DDEBUG
INCLUDES = -O3 -fno-inline -fno-builtin -DW_MEMALLOC -DW_UALIGNED -O3 -I$(SOURCEPATH) $(BLOCK0_I) $(USER0_I) $(USER1_I) $(USER2_I) $(USER3_I) $(USER4_I) $(USER5_I) $(USER6_I)
LINKS =  -L$(BLOCK0_L) -Wl,--start-group -lm -lpthread -lhomesdrWORKINGlibsigsetup_lib.txt -lfftw3f ../asmlib/alibelf32.a -L../libgui/bin -lSDRGUI -lQtGui -lQtCore -lpthread -lqwt -Wl,--end-group

CXX = ccache g++ $(MACH) $(PARAM)
THIS_LIB = $(LIBRARYPATH)libhomesdrWORKINGtestpsetup.txt.a

USER0_I = -I/home/sdr/WORKING/libsig
USER1_I = -I/home/sdr/WORKING/libgui
USER2_I = -I/home/sdr/WORKING/asmlib
USER3_I = -I/usr/lib/qt4/mkspecs/linux-g++
USER4_I = -I/usr/include/QtCore
USER5_I = -I/usr/include/QtGui
USER6_I = -I/usr/include/qwt

BLOCK0_I = -I/home/sdr/WORKING/libsig
BLOCK0_L = /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)

DEP_LIBS = /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/libhomesdrWORKINGlibsigsetup_lib.txt.a \

OBJECT = $(OBJECTPATH)class_example.o \
         $(OBJECTPATH)func_add_asm.o \
	float_dotprod_sse64.o\
	fcomplex_dotprod_sse64.o\

SOURCE = $(SOURCEPATH)class_example.cc \
         $(SOURCEPATH)func_add_asm.cc \

PROG0   = t_libsig
PROG0_l = $(OBJECTPATH)$(PROG0)
OBJECT0 = $(OBJECTPATH)$(PROG0).o
SOURCE0 = $(OBJECTPATH)$(PROG0).cc

PROG1   = t_fftw
PROG1_l = $(OBJECTPATH)$(PROG1)
OBJECT1 = $(OBJECTPATH)$(PROG1).o
SOURCE1 = $(OBJECTPATH)$(PROG1).cc

PROG2   = t_assembly
PROG2_l = $(OBJECTPATH)$(PROG2)
OBJECT2 = $(OBJECTPATH)$(PROG2).o
SOURCE2 = $(OBJECTPATH)$(PROG2).cc

PROG3   = t_memcpy
PROG3_l = $(OBJECTPATH)$(PROG3)
OBJECT3 = $(OBJECTPATH)$(PROG3).o
SOURCE3 = $(OBJECTPATH)$(PROG3).cc

PROG4   = t_time_ms
PROG4_l = $(OBJECTPATH)$(PROG4)
OBJECT4 = $(OBJECTPATH)$(PROG4).o
SOURCE4 = $(OBJECTPATH)$(PROG4).cc

PROG5   = t_sig_overhead
PROG5_l = $(OBJECTPATH)$(PROG5)
OBJECT5 = $(OBJECTPATH)$(PROG5).o
SOURCE5 = $(OBJECTPATH)$(PROG5).cc

PROG6   = t_complex
PROG6_l = $(OBJECTPATH)$(PROG6)
OBJECT6 = $(OBJECTPATH)$(PROG6).o
SOURCE6 = $(OBJECTPATH)$(PROG6).cc

PROG7   = t_memalloc
PROG7_l = $(OBJECTPATH)$(PROG7)
OBJECT7 = $(OBJECTPATH)$(PROG7).o
SOURCE7 = $(OBJECTPATH)$(PROG7).cc

all: checkdir /home/sdr/WORKING/testp/Makefile $(DEP_LIBS) $(THIS_LIB) $(PROG0_l) $(PROG1_l) $(PROG2_l) $(PROG3_l) $(PROG4_l) $(PROG5_l) $(PROG6_l) $(PROG7_l) progs cleanup

homesdrWORKINGlibsigsetup_lib.txt_deps = $(wildcard /home/sdr/WORKING/libsig/*.h) $(wildcard /home/sdr/WORKING/libsig/*.hh) $(wildcard /home/sdr/WORKING/libsig/*.cc) $(wildcard /home/sdr/WORKING/libgui/*.h) $(wildcard /home/sdr/WORKING/libgui/*.hh)
/home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/libhomesdrWORKINGlibsigsetup_lib.txt.a: $(homesdrWORKINGlibsigsetup_lib.txt_deps) /home/sdr/WORKING/libsig/setup_lib.txt
	$(BOLD) && echo "Making in /home/sdr/WORKING/libsig using this setup: setup_lib.txt" && $(NORMAL)
	$(MAKE) -C /home/sdr/WORKING/libsig -f /home/sdr/WORKING/testp/z_lib/z_obj/homesdrWORKINGlibsigsetup_lib.txt.mk
	$(BOLD) && echo "Back to /home/sdr/WORKING/testp" && $(NORMAL)

$(OBJECTPATH)%.d: /home/sdr/WORKING/testp/setup.txt
	rm -f $(OBJECTPATH)$(*F).d

-include $(PROG0_l).d
-include $(PROG1_l).d
-include $(PROG2_l).d
-include $(PROG3_l).d
-include $(PROG4_l).d
-include $(PROG5_l).d
-include $(PROG6_l).d
-include $(PROG7_l).d
-include $(subst $(SOURCEPATH), $(OBJECTPATH), $(SOURCE:.cc=.d))
$(OBJECTPATH)%.o: $(SOURCEPATH)%.cc /home/sdr/WORKING/testp/setup.txt
	$(CXX) -MF $(OBJECTPATH)$(*F).d -MT $(OBJECTPATH)$(*F).o -MM $(FLAGS) $(INCLUDES) $<
	echo "Compiling $<"
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(THIS_LIB): $(OBJECT) /home/sdr/WORKING/testp/setup.txt
	echo "Archiving library $(THIS_LIB)"
	rm -f $(THIS_LIB); \
	ar rc $(THIS_LIB) $(OBJECT); \
	ranlib $(THIS_LIB)

.PHONY: progs
progs:
	rm -f $(PROG0)
	ln -s $(PROG0_l)
	rm -f $(PROG1)
	ln -s $(PROG1_l)
	rm -f $(PROG2)
	ln -s $(PROG2_l)
	rm -f $(PROG3)
	ln -s $(PROG3_l)
	rm -f $(PROG4)
	ln -s $(PROG4_l)
	rm -f $(PROG5)
	ln -s $(PROG5_l)
	rm -f $(PROG6)
	ln -s $(PROG6_l)
	rm -f $(PROG7)
	ln -s $(PROG7_l)

$(PROG0_l): $(OBJECT) $(OBJECT0) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG0)"
	$(CXX) -o $(PROG0_l) $(OBJECT) $(OBJECT0) $(LINKS)

$(PROG1_l): $(OBJECT) $(OBJECT1) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG1)"
	$(CXX) -o $(PROG1_l) $(OBJECT) $(OBJECT1) $(LINKS)

$(PROG2_l): $(OBJECT) $(OBJECT2) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG2)"
	$(CXX) -o $(PROG2_l) $(OBJECT) $(OBJECT2) $(LINKS)

$(PROG3_l): $(OBJECT) $(OBJECT3) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG3)"
	$(CXX) -o $(PROG3_l) $(OBJECT) $(OBJECT3) $(LINKS)

$(PROG4_l): $(OBJECT) $(OBJECT4) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG4)"
	$(CXX) -o $(PROG4_l) $(OBJECT) $(OBJECT4) $(LINKS)

$(PROG5_l): $(OBJECT) $(OBJECT5) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG5)"
	$(CXX) -o $(PROG5_l) $(OBJECT) $(OBJECT5) $(LINKS)

$(PROG6_l): $(OBJECT) $(OBJECT6) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG6)"
	$(CXX) -o $(PROG6_l) $(OBJECT) $(OBJECT6) $(LINKS)

$(PROG7_l): $(OBJECT) $(OBJECT7) $(DEP_LIBS) /home/sdr/WORKING/testp/Makefile
	echo "Building $(SOURCEPATH)$(PROG7)"
	$(CXX) -o $(PROG7_l) $(OBJECT) $(OBJECT7) $(LINKS)

.PHONY: clean
clean:
	echo "Cleaning everything in /home/sdr/WORKING/testp"
	rm -f $(OBJECTPATH)*
	rm -f $(LIBRARYPATH)*
	rm -f $(PROG0)
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f $(PROG3)
	rm -f $(PROG4)
	rm -f $(PROG5)
	rm -f $(PROG6)
	rm -f $(PROG7)

.PHONY: clean_all
clean_all: clean
	echo "Cleaning everything in /home/sdr/WORKING/libsig"
	rm -f /home/sdr/WORKING/libsig/z_lib/z_obj/homesdrWORKINGlibsigsetup_lib.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	rm -f /home/sdr/WORKING/libsig/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	echo "Cleaning everything in /home/sdr/WORKING/libgui"
	rm -f /home/sdr/WORKING/libgui/z_lib/z_obj/homesdrWORKINGlibguisetup_lib.txt/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*
	rm -f /home/sdr/WORKING/libgui/z_lib/$(ARCH)$(MACH)$(DYN)_$(LIBC)/*

.PHONY: cleanup
cleanup:
	touch $(THIS_LIB)

.PHONY: checkdir
checkdir:
	if [ ! -d $(OBJECTPATH) ]; then mkdir -p $(OBJECTPATH); fi
	if [ ! -d $(LIBRARYPATH) ]; then mkdir -p $(LIBRARYPATH); fi

/home/sdr/WORKING/testp/Makefile: /home/sdr/WORKING/testp/setup.txt /home/sdr/WORKING/libsig/setup_lib.txt /home/sdr/WORKING/libgui/setup_lib.txt
	echo "setup or fpga file list has changed: call domakefile before compiling."
	exit 1

