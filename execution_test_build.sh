
cmake --build build --target execution_test

if [ $# -ge 1 ]
then
    build/execution_test $1
fi
