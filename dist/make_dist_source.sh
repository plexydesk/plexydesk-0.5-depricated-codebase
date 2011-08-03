#!/bin/bash

# This script prepares source files from the GIT/base tree
# It uses some env variables that may be set before it is run
# If they are not set it autodetects them from main CMakeLists.txt

color_ok="\\033[1;32m"
color_error="\\033[1;31m"
color_normal="\\033[0;39m"
color_warn="\\033[1;33m"

prog_ok(){
	echo
	echo -e "${color_ok}$@${color_normal}"
	}

prog_err(){
	echo
	echo -e "${color_error}ERROR: $@${color_normal}\n"
	}

prog_warn(){
	echo -e "${color_warn}$@${color_normal}"
	}

check_app_version() {
	_APP_NAME="$( grep -e '\W*SET\W*(\W*APPLICATION_EXE_NAME' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*APPLICATION_EXE_NAME\W*(\w*)\W*\)/\1/' )"

	_APP_VERSION_MAJOR="$( grep -e '\W*SET\W*(\W*LIB_MAJOR' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_MAJOR\W*(\w*)\W*\)/\1/' )"
	_APP_VERSION_MINOR="$( grep -e '\W*SET\W*(\W*LIB_MINOR' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_MINOR\W*(\w*)\W*\)/\1/' )"
	_APP_VERSION_RELEASE="$( grep -e '\W*SET\W*(\W*LIB_RELEASE' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_RELEASE\W*(\w*)\W*\)/\1/' )"
	_APP_VERSION="${_APP_VERSION_MAJOR}.${_APP_VERSION_MINOR}.${_APP_VERSION_RELEASE}"
	}


CUR_DIR=`pwd`
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
GIT_DIR="$( dirname "${SCRIPT_DIR}" )"

_READY_DIR="${GIT_DIR}/INSTALLERS"
check_app_version

if [ -z "${READY_DIR}" ] ; then
	prog_warn "The shell variable READY_DIR is not set. Using the built-in one... (${_READY_DIR})"
	READY_DIR="${_READY_DIR}"
fi
if [ -z "${APP_NAME}" ] ; then
	prog_warn "The shell variable APP_NAME is not set. Using the built-in one... (${_APP_NAME})"
	APP_NAME="${_APP_NAME}"
fi
if [ -z "${APP_VERSION}" ] ; then
	prog_warn "The shell variable APP_VERSION is not set. Using the built-in one... (${_APP_VERSION})"
	APP_VERSION="${_APP_VERSION}"
fi

TRANSFORM_STRING="s,^\./,${APP_NAME}-${APP_VERSION}/,"
TRANSFORM_STRING_DOT="s,^\.$,${APP_NAME}-${APP_VERSION},"


# We will work from the base GIT folder
cd "${GIT_DIR}"

# Ensure we are in the base folder
if ! [ -f 'AUTHORS' ]; then
	prog_err "The $(pwd) is not the base ${APP_NAME} folder. ABORTING..."
	cd "${CUR_DIR}"
	exit 1
fi

# Ensure we have all folders created
if ! [ -d "${READY_DIR}" ]; then
	prog_ok "Creating ${READY_DIR} folder..."
	mkdir -p "${READY_DIR}"
fi

# Create the tarball
prog_ok "Creating the sources tarball..."
tar -cjf "${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2" . --exclude-vcs --exclude 'build' --exclude 'INSTALLERS' --transform "${TRANSFORM_STRING}" --transform "${TRANSFORM_STRING_DOT}" --numeric-owner

if [ $? -ne 0 ]; then
    prog_err "Creating the sources tarball..."
	cd "${CUR_DIR}"
	exit 1
fi

prog_ok "Dist Source file is ready. You can find it here:"
prog_warn "  ${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2"

cd "${CUR_DIR}"
