#!/bin/bash

# This script prepares deb files from the GIT/base tree
# It uses some env variables that may be set before it is run
# If they are not set it autodetects them from main CMakeLists.txt
# If you have the GPGKEY env var set you will get the deb sources,
# otherwise you get the binary

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

# Ensure we have the debuild binary
if ! eval "which debuild > /dev/null 2>&1"; then
	prog_err "It seems that you do not have the 'debuild' binary in your PATH. ABORTING..."
	exit 1
fi

CUR_DIR=`pwd`
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
GIT_DIR="$( dirname "${SCRIPT_DIR}" )"

_BUILD_DIR="${GIT_DIR}/build"
_READY_DIR="${GIT_DIR}/INSTALLERS"
check_app_version

if [ -z "${BUILD_DIR}" ] ; then
	prog_warn "The shell variable BUILD_DIR is not set. Using the built-in one... (${_BUILD_DIR})"
	export BUILD_DIR="${_BUILD_DIR}"
fi
if [ -z "${READY_DIR}" ] ; then
	prog_warn "The shell variable READY_DIR is not set. Using the built-in one... (${_READY_DIR})"
	export READY_DIR="${_READY_DIR}"
fi
if [ -z "${APP_NAME}" ] ; then
	prog_warn "The shell variable APP_NAME is not set. Using the built-in one... (${_APP_NAME})"
	export APP_NAME="${_APP_NAME}"
fi
if [ -z "${APP_VERSION}" ] ; then
	prog_warn "The shell variable APP_VERSION is not set. Using the built-in one... (${_APP_VERSION})"
	export APP_VERSION="${_APP_VERSION}"
fi

DEB_BUILD_DIR_="${BUILD_DIR}/deb-build"

# We will work from the base GIT folder
cd "${GIT_DIR}"

# Ensure we are in the base folder
if ! [ -f 'AUTHORS' ]; then
	prog_err "The $(pwd) is not the base ${APP_NAME} folder. ABORTING..."
	cd "${CUR_DIR}"
	exit 1
fi

# Ensure we have all folders created
if ! [ -d "${READY_DIR}/deb" ]; then
	prog_ok "Creating ${READY_DIR}/deb folder..."
	mkdir -p "${READY_DIR}/deb"
fi
if [ -d "${DEB_BUILD_DIR_}" ]; then
	prog_ok "Cleaning up the DEB build folder ${DEB_BUILD_DIR_}..."
	rm -fR "${DEB_BUILD_DIR_}"
fi
prog_ok "Creating ${DEB_BUILD_DIR_} folder..."
mkdir -p "${DEB_BUILD_DIR_}"

# Check if source tarball exists and copy it to the deb build folder
if ! [ -f "${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2" ]; then
	dist/make_dist_source.sh

	if [ $? -ne 0 ]; then
		prog_err "Creating the dist DEB files at stage: dist source tarball prepare..."
		cd "${CUR_DIR}"
		exit 1
	fi
fi

prog_ok "Copying the dist source tarball to $(pwd)/build/deb-build folder..."
cp "${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2" "${DEB_BUILD_DIR_}/${APP_NAME}_${APP_VERSION}.tar.bz2"

prog_ok "Unpacking the source tarball to ${DEB_BUILD_DIR_}/${APP_NAME}-${APP_VERSION}..."
cd "${DEB_BUILD_DIR_}"
tar xjf "${APP_NAME}_${APP_VERSION}.tar.bz2"
cd "${DEB_BUILD_DIR_}/${APP_NAME}-${APP_VERSION}"

if [ -z "${GPGKEY}" ] ; then
	prog_ok "Building the dist DEB binary file with debuild..."
	debuild -b -us -uc
else
	prog_ok "Building the dist DEB source files only with debuild..."
	debuild -k${GPGKEY} -S -sa -Zbzip2
fi

if [ $? -ne 0 ]; then
    prog_err "Creating the dist DEB files..."
	cd "${CUR_DIR}"
	exit 1
fi

prog_ok "Copying the dist DEB files to ${READY_DIR}/deb folder..."
if [ -z "${GPGKEY}" ] ; then
	cp ${DEB_BUILD_DIR_}/${APP_NAME}_${APP_VERSION}_*.deb "${READY_DIR}/deb"
else
	cp ${DEB_BUILD_DIR_}/${APP_NAME}_${APP_VERSION}*.changes "${READY_DIR}/deb"
	cp ${DEB_BUILD_DIR_}/${APP_NAME}_${APP_VERSION}*.dsc "${READY_DIR}/deb"
	cp ${DEB_BUILD_DIR_}/${APP_NAME}_${APP_VERSION}*.bz2 "${READY_DIR}/deb"
fi

prog_ok "Cleaning up the DEB build folder ${DEB_BUILD_DIR_}..."
rm -fR "${DEB_BUILD_DIR_}"


prog_ok "Dist DEB files are ready."
if [ -z "${GPGKEY}" ] ; then
	prog_warn "Don't forget to sign them with 'debsign *.changes' if they will be distributed."
fi
prog_warn "You can find the dist DEB files here:\n  ${READY_DIR}/deb\n"

ls -la "${READY_DIR}/deb"

cd "${CUR_DIR}"
