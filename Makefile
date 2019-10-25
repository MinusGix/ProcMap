.PHONY: build_debug clean

output_folder = build
output = $(output_folder)/program

source_files = src/main.cpp src/procmaps.cpp


build_debug:
	mkdir -p $(output_folder)
	clang++ -std=c++17 -DDEBUG $(source_files) -o $(output) -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors -Wno-newline-eof -Wno-missing-prototypes

clean:
	rm $(output)