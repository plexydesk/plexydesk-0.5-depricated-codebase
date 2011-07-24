#!/bin/bash

# This script prepares source files from the GIT tree
# It uses some env variables that have to be set before it is run

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


CUR_DIR=`pwd`
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
GIT_DIR="$( dirname "${SCRIPT_DIR}" )"

_READY_DIR="${GIT_DIR}/INSTALLERS"
_APP_NAME="plexydesk"
_APP_VERSION="0.6.0"

if [ -z "${READY_DIR}" ] ; then
	prog_warn "The shell variable READY_DIR is not set. Using the built-in one..."
	READY_DIR="${_READY_DIR}"
fi
if [ -z "${APP_NAME}" ] ; then
	prog_warn "The shell variable APP_NAME is not set. Using the built-in one..."
	APP_NAME="${_APP_NAME}"
fi
if [ -z "${APP_VERSION}" ] ; then
	prog_warn "The shell variable APP_VERSION is not set. Using the built-in one..."
	APP_VERSION="${_APP_VERSION}"
fi

TRANSFORM_STRING="s,^\./,${APP_NAME}-${APP_VERSION}/,"
TRANSFORM_STRING_DOT="s,^\.$,${APP_NAME}-${APP_VERSION},"


# We will work from the base GIT folder
cd ${GIT_DIR}

# Ensure we are in the GIT repo
if ! [ -d '.git' ]; then
	prog_err "The $(pwd) is not a GIT directory. ABORTING..."
	cd ${CUR_DIR}
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
	cd ${CUR_DIR}
	exit 1
fi

prog_ok "Dist Source file is ready. You can find it here:"
prog_warn "  ${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2"

cd ${CUR_DIR}
