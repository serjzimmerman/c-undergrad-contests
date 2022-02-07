# dictionary

## 1. Compilation

### Linux
```sh
# Clone repository and navigate to the dictionary directory
git clone https://github.com/serjzimmerman/mipt-vladimirov-contest.git
cd path/to/counter

cmake -S ./ -B build/ DCMAKE_BUILD_TYPE=Debug
# Build in debug or release configuration. Debug build is compiled with -pg flags for GNU profiler
cd build/
make install
# Installs wordcount application to app/bin/ and two test programs to lib/hashtable/test/bin/

# To individually 
```

## 2. Wordcount application

Application using hashtable module to count occurrences of words in text files and display results in sorted order
```sh
./app/bin/wordcount -h
# Usage: wordcounter [-l] [-i <path>] [-o <path>], -i for input file, -o for output file (default: prints to stdout), -l to lowercase all characters
```

## 3. Running tests for hashtable module
```sh
# Navigate to lib/hashtable directory
cd lib/hashtable

# Run all tests for both targets: testmemoryoptimized, testspeedoptimized
./run_tests.sh

# Run individual test cases with lib/hashtable/test/testmemoryoptimized(testspeedoptimized)
./lib/hashtable/test/bin/testmemoryoptimized -h
# Usage: test [-i <path>]
# You can provide test file path with argument -i for large test cases (file is then memory mapped). Otherwise program accepts input from stdin

# To generate new tests run script
rm cases/*.dat
./gentest.py -h
# gentest.py -n <number_of_tests>
```