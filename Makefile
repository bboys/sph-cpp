####### Compiler, tools and options
ifeq ("$(shell uname)", "Linux")
  ifeq (,$(findstring i686,$(shell uname -m)))
    OS=linux
  else
    OS=wingtip
  endif
endif

ifeq ("$(shell uname)", "Darwin")
  OS=OSX
endif

ifndef OS
  $(error Unsupported operating system)
endif

BUILDDIR =	build-$(OS)
ifeq ($(OS), OSX)
    NVSDKINC	=	-I/usr/X11R6/include -I./graphics/cg -I./
    NVSDKLIB	=	-framework Cg -framework OpenGL -lglut -lGLU -lGLEW -lpng -lpthread
    CXX		=	g++-4.7
    CXXFLAGS	=	$(APP_FLAGS) -g -std=c++11 -fopenmp
    LINK	=	g++-4.7
    LIBS	=	-L/usr/gcc-4.7.1/lib -L/usr/lib -L/usr/local/lib -lm
else
    NVSDKINC	=	-I./graphics/cg -I./
    NVSDKLIB	=	-lCgGL -lCg -lglut -lGLU -lGLEW -lpng -lpthread -lGL
    CXX		=	g++
    CXXFLAGS	=	$(APP_FLAGS) -g -std=c++11 -fopenmp
    LINK	=	g++
    LIBS	=	-L/usr/lib -L/usr/local/lib -lm 
endif

REGULAR = 	-O3 -pipe -Wall -fno-strength-reduce -fPIC
APP_FLAGS=	-Wall -DUNIX
INCPATH	=	$(NVSDKINC)
LFLAGS	= 	-fPIC -g
TAR	=	tar -cf
GZIP	=	gzip -9f
NASM	=	nasm -f elf 
LIBMESA	= 	$(LIB) -lGLU -lglut 
LIBX	=	-L/usr/X11R6/lib -lX11 -lXmu -lXi -lgomp
LIBDIR	=	$(LIBMESA) $(LIBX) $(LIBS) $(NVSDKLIB)

####### Files

include Make.inc

OBJ = $(addprefix $(BUILDDIR)/, $(OBJECTS))
TGT = $(addprefix $(BUILDDIR)/, $(TARGET))

all: checkdirs $(TGT)

#~ all: checkdirs $(TGT) strip

prof: checkdirs proflink $(TGT)

.SUFFIXES:

$(BUILDDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $< -o $@

$(TGT): $(OBJ) 
	$(LINK) $(LFLAGS) -o $(TGT) $(OBJ) $(LIBDIR)

checkdirs: $(BUILDDIR)

strip:
	strip $(TGT)

proflink:
	LFLAGS	= 	-fPIC -pg

$(BUILDDIR):
	@mkdir -p $@/graphics/cg
	@mkdir -p $@/graphics/opengl

clean:
	-rm -rf $(BUILDDIR)
