
cmake --build build --target parsing_test.out

if [ $# -ge 1 ]
then
    build/parsing_test.out /home/sofiane/Documents/PROJETS/PROGRAMMING_LANGUAGE/ressources/mxp_files/$1
fi
