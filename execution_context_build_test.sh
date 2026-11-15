
cmake --build build --target execution_context_test

if [ $# -ge 1 ]
then
    build/execution_context_test $1
fi
