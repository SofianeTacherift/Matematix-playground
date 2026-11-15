
cmake --build build --target lexing_test

if [ $# -ge 1 ]
then
    cat $1
    build/lexing_test $1

fi
