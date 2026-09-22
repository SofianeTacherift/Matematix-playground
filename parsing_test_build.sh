
cmake --build build --target parsing_test.out

if [ $# -ge 1 ]
then
    build/parsing_test.out $1
fi
