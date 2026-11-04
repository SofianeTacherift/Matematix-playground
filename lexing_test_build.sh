
cmake --build build --target lexing_test.out

if [ $# -ge 1 ]
then
    cat $1
    build/lexing_test.out $1

fi
