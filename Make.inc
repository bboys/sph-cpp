SOURCES	=	particle.cpp bucket.cpp particlesource.cpp particleiterator.cpp
SOURCES+=	graphics/base/scene.cpp   graphics/base/surface.cpp
SOURCES+=	graphics/opengl/scene.cpp graphics/opengl/particle.cpp graphics/opengl/plane.cpp
SOURCES+=	graphics/cg/scene.cpp     graphics/cg/particle.cpp     graphics/cg/plane.cpp     graphics/cg/shader.cpp
OBJECTS	=	$(SOURCES:.cpp=.o)
TARGET	= 	ray
