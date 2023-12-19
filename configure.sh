#!/usr/bin/sh

## Configuration (x64/x86/...)
TARGET_CONFIGURATION=$(uname -m)

## Platform (OS) detection
TARGET_PLATFORM=$OSTYPE

## Setup bootstrap build directory
BOOTSTRAP_BUILD_DIR=bootstrap-${TARGET_CONFIGURATION}-${TARGET_PLATFORM}

## Setup bootstrap source directory
BOOTSTRAP_SOURCE_DIR=source/bootstrap

## Setup bootstrap build system
BOOTSTRAP_BUILD_TOOL=Ninja

## Create bootstrap build directory, if requred
if [ ! -d ${BOOTSTRAP_BUILD_DIR} ]; then 
  mkdir ${BOOTSTRAP_BUILD_DIR}
fi

## Configure CMake for bootstrap build
cmake -B ${BOOTSTRAP_BUILD_DIR} -S ${BOOTSTRAP_SOURCE_DIR} -G ${BOOTSTRAP_BUILD_TOOL}
