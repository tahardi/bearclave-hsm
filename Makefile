# https://clarkgrubb.com/makefile-style-guide
MAKEFLAGS += --warn-undefined-variables
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.DEFAULT_GOAL := pre-pr
.DELETE_ON_ERROR:
.SUFFIXES:

default_build=build-debug
include_dir=./include
src_dir=./src
test_dir=./tests

header_files := $(shell find $(include_dir) -name '*.h')
source_files := $(shell find $(src_dir) -name '*.c')
test_files := $(shell find $(test_dir) -name '*.c')

.PHONY: pre-pr
pre-pr: fmt lint test

.PHONY: build
build: $(default_build)

.PHONY: build-%
build-%:
	@cmake --preset $*
	@cmake --build --preset build-$*

.PHONY: fmt
fmt:
	@clang-format -i \
		$(header_files) \
		$(source_files) \
		$(test_files)

.PHONY: lint
lint:
	@clang-tidy \
		--config-file .clang-tidy \
		--quiet \
		-p ./$(default_build) \
		$(source_files)

.PHONY: test
test: test-debug \
	test-sanitize-asan-ubsan \
	test-sanitize-msan \
	test-sanitize-tsan

.PHONY: test-%
test-%: build-%
	@echo "Testing preset: $*"
	@cmake --build ./build-$* --target test

.PHONY: clean
clean:
	@rm -rf ./build-*
