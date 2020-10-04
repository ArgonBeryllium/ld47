CXX=clang++
MINGW=x86_64-w64-mingw32-g++
EMXX=em++

LIBS=-lSDL2 -lbj -lSDL2_mixer -lSDL2_image 
WINLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
CXXFLAGS=--std=c++17
MINGWFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
FILES=src/*.cpp

test: $(FILES)
	figlet "LD47" -fshadow | lolcat
	echo "---\nREMINDER: optimisation is off by default.\n---"
	$(CXX) $(FILES) $(LIBS) $(CXXFLAGS) -o bin/test

test-win: $(FILES)
	figlet "LD47 [WIN]" -fshadow | lolcat
	mkdir src/bj
	cp ../../cpp/berryJam/src/* -r src/bj/
	rm src/bj/test.cpp
	$(MINGW) $(FILES) src/bj/*.cpp src/bj/**/*.cpp $(WINLIBS) $(CXXFLAGS) $(MINGWFLAGS) -o bin/win/test.exe
	rm -rf src/bj
