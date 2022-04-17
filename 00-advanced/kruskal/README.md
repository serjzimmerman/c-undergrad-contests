# kruskal

Kruskal's algorithm for finding spanning tree with minimum total edge weight

## 1. Compilation

### Linux
```sh
# Clone repository and navigate to the dictionary directory
git clone https://github.com/serjzimmerman/mipt-vladimirov-contest.git
cd path/to/kruskal

cmake -S ./ -B build/ DCMAKE_BUILD_TYPE=Debug -DVERBOSE=On
# Build in debug or release configuration. Set VERBOSE option to ON for VERBOSE errors
cd build/
make install
```

## 2. Running tests for hashtable module
```sh
# Run all tests for both kruskal target. Test cases are taken from the contest problem
./run_tests.sh
```