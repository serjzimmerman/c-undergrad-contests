_base="./cases"

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

for file in $_base/test*.dat
do
  count=`echo $file | egrep -o [0-9]+`
  echo -n "Testing ${green}${file}${reset} ... "
  ./test/bin/test -i $file > $_base/temp.dat
  if diff -Z ${_base}/ans${count}.dat ${_base}/temp.dat; then
    echo "${green}Passed${reset}"
  else
    echo "${red}Failed${reset}"
    result=`cat ${_base}/temp.dat`
    expected=`cat ${_base}/ans${count}.dat`
    echo -e "Result: \t$result"
    echo "--------------------------------"
    echo -e "Expected: \t$expected"
  fi
done