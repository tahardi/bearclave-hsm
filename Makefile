# https://clarkgrubb.com/makefile-style-guide
MAKEFLAGS += --warn-undefined-variables
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.DEFAULT_GOAL := pre-pr
.DELETE_ON_ERROR:
.SUFFIXES:

.PHONY: pre-pr
pre-pr: fmt lint

build_dir=./build-debug
include_dir=./include
src_dir=./src
test_dir=./tests

.PHONY: fmt
fmt:
	clang-format -i \
		$(include_dir)/*.h \
		$(src_dir)/*.c \
		$(test_dir)/*.c

.PHONY: lint
lint:
	clang-tidy \
		--config-file .clang-tidy \
		-p $(build_dir) \
		$(src_dir)/*.c
