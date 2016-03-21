all:
	cd ./lab_4 && make
	cd ./lab_5 && make

test:
	cd ./lab_4 && make test
	cd ./lab_5 && make test

clean:
	cd ./lab_4 && make clean
	cd ./lab_5 && make clean
