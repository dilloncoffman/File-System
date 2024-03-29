# Dillon Coffman, Section 1 - Operating Systems Project 4
# CompLab Tutor: Chenglong Fu
all:
	gcc -o fs fs.c

test:
	gcc -o testfs fs.c -D TESTING
	./testfs

test_part:
	gcc -o testingpart fs.c -D TESTING_PART
	./testingpart

clean: 
	rm -rf fs
	rm -rf testfs
	rm -rf testingpart