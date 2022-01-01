OBJS = main.o robotlog.o plan.o log.o csvexport.o robot-math/steve.o VikingInstructionExport.o
CXX = g++
CXXFLAGS = -Og -g -std=c++20 `pkg-config gtkmm-3.0 --cflags` -Wall -Wextra

all: robotlog

robotlog: $(OBJS)
	$(CXX) $(OBJS) -o robotlog `pkg-config gtkmm-3.0 --libs`

win: $(CPPSRC)
	$(CXX) $(OBJS) -o robotlog `pkg-config gtkmm-3.0 --libs` -mwindows

run: robotlog
	./robotlog

clean:
	rm -f robotlog $(OBJS)
