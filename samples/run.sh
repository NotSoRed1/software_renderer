
clang++ -std=c++20 "$1".cpp -o build/"$1".exe -ffast-math -O3 -DNDEBUG -luser32.lib -lGdi32.lib
./build/"$1".exe