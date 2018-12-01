# Dillon Coffman, Section 1 - Operating Systems Project 4
# CompLab Tutor: Chenglong Fu
all:
	gcc -o fs fs.c helpers.c

test:
	gcc -o testfs fs.c helpers.c -D TESTING
	./testfs

test_part:
	gcc -o testingpart fs.c helpers.c -D TESTING_PART
	./testingpart

clean: 
	rm -rf fs
	rm -rf testfs
	rm -rf testingpart