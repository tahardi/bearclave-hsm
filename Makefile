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

header_files := $(shell find $(include_dir) -name '*.h')
source_files := $(shell find $(src_dir) -name '*.c')
test_files := $(shell find $(test_dir) -name '*.c')

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
		-p $(build_dir) \
		$(source_files)
