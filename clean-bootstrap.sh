#!/usr/bin/sh

## Configuration (x64/x86/...)
TARGET_CONFIGURATION=$(uname -m)

## Platform (OS) detection
TARGET_PLATFORM=$OSTYPE

## Setup bootstrap build directory
BOOTSTRAP_BUILD_DIR=bootstrap-$TARGET_CONFIGURATION-$TARGET_PLATFORM

## Perform a clean operation
rm -rf $BOOTSTRAP_BUILD_DIR
mkdir -p $BOOTSTRAP_BUILD_DIR
./configure.sh
