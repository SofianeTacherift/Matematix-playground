
cmake --build build --target matpx

if [ $# -ge 2 ]
then
    build/matpx $1 $2

fi
