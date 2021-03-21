clear
mkdir -p build
rm -f ./build/*
gcc -Wextra test.c ./lib/cbuffer.c -I./lib -lunity -o ./build/test
./build/test
