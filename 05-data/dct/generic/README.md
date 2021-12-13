# counter

String frequency counter module using hashtable. Hash function used is [djb2](http://www.cse.yorku.ca/~oz/hash.html).

## 1. Compilation

### Linux
```sh
# Clone repository and navigate to the filesort directory
git clone https://github.com/serjzimmerman/mipt-vladimirov-contest.git
cd path/to/counter

cmake -S ./ -B build/
cd build/
make install
# Installs to root repository directory
```

## 2. Generating and running tests

```sh
# Generaing random tests with python script
./gentest.py
Usage: gentest.py -n <number_of_tests>

# Running tests with bash(difference checking is incredibly primitive)
./run_tests.sh

# Main application accepts the same input parameters as the hwh problem in the contest
./counter
```