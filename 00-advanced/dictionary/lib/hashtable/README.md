# hashtable

String frequency counter module using hashtable. Hash function used is [djb2](http://www.cse.yorku.ca/~oz/hash.html).

## 1. Generating and running tests

```sh
# Generaing random tests with python script
./gentest.py
Usage: gentest.py -n <number_of_tests>

# Running tests with bash(difference checking is incredibly primitive)
./run_tests.sh

# Main application accepts the same input parameters as the hwh problem in the contest
./test
```