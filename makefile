CXX=clang++
MINGW=x86_64-w64-mingw32-g++
EMXX=em++

LIBS=-lSDL2 -lbj 
CXXFLAGS=--std=c++17
MINGWFLAGS=-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
EMXXFLAGS=-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png","jpg"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file res

FILES=src/*.cpp

test: $(FILES)
	figlet "LD47" -fshadow | lolcat
	echo "---\nREMINDER: optimisation is off by default.\n---"
	$(CXX) $(FILES) $(LIBS) $(CXXFLAGS) -o bin/test

test-win: $(FILES)
	figlet "LD47 [WIN]" -fshadow | lolcat
	$(MINGW) $(FILES) $(LIBS) $(CXXFLAGS) $(MINGWFLAGS) -o bin/win/test.exe

test-web-wasm: $(FILES)
	figlet "LD47 [WEB/WASM]" -fshadow | lolcat
	$(EMXX) -s WASM=1  $(FILES) $(CXXFLAGS) $(EMXXFLAGS) -o bin/web/wasm/index.html
test-web-js: $(FILES)
	figlet "LD47 [WEB/JS]" -fshadow | lolcat
	$(EMXX) -s WASM=0 $(FILES) $(CXXFLAGS) $(EMXXFLAGS) -o bin/web/js/index.html
