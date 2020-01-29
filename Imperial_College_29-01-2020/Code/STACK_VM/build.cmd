clang main.cpp -o Stack_VM.exe -O3
emcc -o Stack_VM.js main.cpp -O3 -s "EXPORTED_FUNCTIONS=['_test']" --preload-file .\memory.bin --preload-file .\program.bin