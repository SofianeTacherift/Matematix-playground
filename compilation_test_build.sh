
cmake --build build --target compilation_test.out

if [ $# -ge 1 ]
then
    build/compilation_test.out $1
fi
