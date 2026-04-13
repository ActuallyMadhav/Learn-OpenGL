mkdir -p build
cd build
cmake ..

cmake --build . --parallel 8

if [ $? -eq 0 ]; then
    echo "---------------------------------"
    ./app
else
    echo "Build failed. See errors above"
fi