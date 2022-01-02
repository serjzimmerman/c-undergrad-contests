for file in ./*
do
	cd $file && make clang-format && make clean || cd ..
done
