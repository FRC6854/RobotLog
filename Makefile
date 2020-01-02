CPPSRC = main.cpp robotlog.cpp plan.cpp log.cpp

all: robotlog

robotlog:
	g++ $(CPPSRC) robot-math/steve.cpp -o robotlog `pkg-config gtkmm-3.0 --cflags --libs`

clang:
	clang++ $(CPPSRC) robot-math/steve.cpp -o robotlog `pkg-config gtkmm-3.0 --cflags --libs`

win:
	g++ $(CPPSRC) robot-math/steve.cpp -o robotlog `pkg-config gtkmm-3.0 --cflags --libs` -mwindows

run: robotlog
	./robotlog

clean:
	rm -f robotlog
