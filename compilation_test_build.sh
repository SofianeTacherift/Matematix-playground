
cmake --build build --target compilation_test

if [ $# -ge 1 ]
then
    build/compilation_test $1
fi
