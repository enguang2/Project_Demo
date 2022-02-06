CXX=g++
CXX_FLAGS=-std=c++11 -I/usr/include/SDL2/ -I. -Iassets/ -Iassets/BOSS/ -Iassets/NAV/ -lSDL2main -lSDL2 -lSDL2_image
DBGCFLAGS=-g -O0 -DDEBUG ${CXX_FLAGS}

debug: bin/debug
exec: bin/exec
tests: bin/tests

bin/tests: test.cpp base.cpp gamemaster.cpp assets/*.cpp assets/BOSS/*.cpp assets/NAV/*.cpp 
	#mkdir bin
	${CXX} test.cpp base.cpp gamemaster.cpp assets/*.cpp assets/BOSS/*.cpp assets/NAV/*.cpp ${DBGCFLAGS} -o bin/tests

bin/debug: main.cpp base.cpp gamemaster.cpp assets/*.cpp assets/BOSS/*.cpp assets/NAV/*.cpp
	#mkdir bin
	${CXX} main.cpp base.cpp gamemaster.cpp assets/*.cpp assets/BOSS/*.cpp assets/NAV/*.cpp ${DBGCFLAGS} -o bin/debug

bin/exec: main.cpp base.h base.cpp gamemaster.h gamemaster.cpp assets/*.h assets/*.cpp assets/BOSS/*.h assets/BOSS/*.cpp assets/NAV/*.h assets/NAV/*.cpp
	#mkdir bin
	${CXX} main.cpp base.cpp gamemaster.cpp assets/*.cpp assets/BOSS/*.cpp assets/NAV/*.cpp ${CXX_FLAGS} -o bin/exec

clean:
	rm -rf bin/*
