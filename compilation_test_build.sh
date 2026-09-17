
cmake --build build --target compilation_test.out

if [ $# -ge 1 ]
then
    build/compilation_test.out /home/sofiane/Documents/PROJETS/PROGRAMMING_LANGUAGE/ressources/mxp_files/$1
fi
