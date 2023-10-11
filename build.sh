cmake -S . -B build
cmake --build build
if [[ "$OSTYPE" == "darwin"* ]]; then
  ./build/smol-graph
elif [[ "$OSTYPE" == "msys" ]]; then
  ./build/smol-graph.exe
fi
