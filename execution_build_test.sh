
cmake --build build --target execution_test.out

if [ $# -ge 1 ]
then
    build/execution_test.out $1
fi
