CC = gcc
CFLAGS = -Og -g -std=c99 -Wall -Wextra

BUILD_DIR = build

all: test

${BUILD_DIR}/argparse_test: argparse_test.c ${BUILD_DIR} ../argparse.h
	${CC} ${CFLAGS} $< -o $@

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

.PHONY: test
test: ${BUILD_DIR}/argparse_test
	${BUILD_DIR}/argparse_test

.PHONY: clean
clean:
	rm ${BUILD_DIR}/argparse_test
