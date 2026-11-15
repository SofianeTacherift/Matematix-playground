cmake --build build --target parsing_test

if [ $# -ge 1 ]
then
    build/parsing_test $1
fi
