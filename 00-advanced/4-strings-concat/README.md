# dictionary

## 1. Task

[*Input*]: an array (strings[]) of strings (roughly ~10-100k)

Find all quadruplets of such strings *a*, *b*, *c*, *d* from strings[]j that a + b == c + d with assymptotic complexity of roughly O($n^2$)

[*Output*]: print all found quadruplets *a*, *b*, *c*, *d*

## 2. Compilation

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
```