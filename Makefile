SRCS := main.cpp \
	BaseRenderer.cpp \
	CudaRenderer.cu \
	DecoratorRenderer.cpp \
	GLRenderer.cpp \
	HiddenLineRemovalRenderer.cpp \
	Matrix.cpp \
	Point.cpp \
	Renderer.cpp \
	WireframeRenderer.cpp

OBJS := $(patsubst %.cu,%.o,$(patsubst %.cpp,%.o,$(SRCS)))

%.o: %.cu
	nvcc -c -o $@ $< $(CFLAGS)

3dRenderer: $(OBJS)
	gcc -o $@ $(OBJS) -lGLU -lGL -lglut -lcudart -lstdc++ -lm

.PHONY: clean
clean:
	$(RM) $(OBJS) 3dRenderer

.PHONY: all
all: 3dRenderer
