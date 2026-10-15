
cmake --build build --target execution_test.out

if [ $# -ge 1 ]
then
    build/execution_test.out /home/sofiane/Documents/PROJETS/PROGRAMMING_LANGUAGE/ressources/mxp_files/$1
fi
