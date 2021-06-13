CPPSRC = main.cpp robotlog.cpp plan.cpp log.cpp robot-math/steve.cpp
CXX = g++
CXXFLAGS = -O2 -g

all: robotlog

robotlog: $(CPPSRC)
	$(CXX) $(CPPSRC) $(CXXFLAGS) -o robotlog `pkg-config gtkmm-3.0 --cflags --libs`

win: $(CPPSRC)
	$(CXX) $(CPPSRC) $(CXXFLAGS) -o robotlog `pkg-config gtkmm-3.0 --cflags --libs` -mwindows

run: robotlog
	./robotlog

clean:
	rm -f robotlog
