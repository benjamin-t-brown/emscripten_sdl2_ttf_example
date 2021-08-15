FLAGS=-g -Wall -std=c++17
CODE=Test.cpp
SRC=$(CODE:.cpp=.o)
EMCC_LIBS=-g -O3 -s USE_SDL=2 -s USE_SDL_TTF=2 -s ALLOW_MEMORY_GROWTH=1 -s SAFE_HEAP=1 -s WARN_UNALIGNED=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=2 
EMCC_EXPORTED=-s EXPORTED_FUNCTIONS='["_main"]' -s EXPORTED_RUNTIME_METHODS=['ccall']

EXE=test

ifeq ($(OS),Windows_NT)
  LIBS= -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
else
  LIBS= -lSDL2main -lSDL2 -lSDL2_ttf
endif

.PHONY: $(EXE)

$(EXE): $(SRC)
	g++ $(FLAGS) $(SRC) -o $(EXE) $(LIBS)

temp:
	mkdir -p .build

web: temp
	em++ $(CODE) $(EMCC_LIBS) $(EMCC_EXPORTED) --preload-file assets -o .build/$(EXE).js
	mkdir -p web
	mv -v .build/* web
	rm -fd .build

run: $(EXE)
	./$(EXE)

.cpp.o:
	g++ $(FLAGS) -c $(@:.o=.cpp) -o $@

clean:
	rm -f *.o
	rm -rf $(EXE)
	rm -rf web/*.data
	rm -rf web/*.js
	rm -rf web/*.wasm