
cmake --build build --target lexing_test.out

if [ $# -ge 1 ]
then
    build/lexing_test.out /home/sofiane/Documents/PROJETS/PROGRAMMING_LANGUAGE/ressources/mxp_files/$1
fi
