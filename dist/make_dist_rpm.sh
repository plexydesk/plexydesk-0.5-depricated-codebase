#!/bin/bash

# This script prepares rpm files from the GIT/base tree
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

create_rpm_structures(){
	cd "${BUILD_DIR}"

	cleanup_rpmrc_additions

	if [ -d 'rpm' ]; then
		prog_ok "Cleaning up the whole $(pwd)/rpm folder..."
		rm -fR rpm
	fi

	prog_ok "Creating the necessary folder structure under the $(pwd)/rpm folder..."
	mkdir rpm
	mkdir rpm/BUILD
	mkdir rpm/RPMS
	mkdir rpm/RPMS/i586
	mkdir rpm/RPMS/x86_64
	mkdir rpm/RPMS/noarch
	mkdir rpm/SOURCES
	mkdir rpm/SPECS
	mkdir rpm/SRPMS
	mkdir rpm/tmp

	prog_ok "Adding the necessary rpm resources to ${HOME}/.rpmrc..."

cat <<END >>"${HOME}/.rpmrc"
###plexydesk rpmbuild additions###
# These will be autoremoved when rpmbuild is ready
# Please do not modify the lines starting with ###
buildarchtranslate: i386: i586
buildarchtranslate: i486: i586
buildarchtranslate: i586: i586
buildarchtranslate: i686: i586
###plexydesk rpmbuild additions###
END

	prog_ok "Adding the necessary rpm resources to ${HOME}/.rpmmacros..."

cat <<END >>"${HOME}/.rpmmacros"
###plexydesk rpmbuild additions###
# These will be autoremoved when rpmbuild is ready
# Please do not modify the lines starting with ###
%_topdir                $(pwd)/rpm
%_tmppath               $(pwd)/rpm/tmp

# Add your name and e-mail into the %packager field below. You may also want to
# replace vendor with yourself.
%packager               PlexyDesk Team <webmaster@plexyplanet.org>
%vendor                 PlexyDesk Team
###plexydesk rpmbuild additions###
END

	# Ensure we go back to base GIT folder
	cd "${GIT_DIR}"
	}

cleanup_rpmrc_additions(){
	if [ -f "${HOME}/.rpmrc" ]; then
		prog_ok "Removing the added rpm resources from ${HOME}/.rpmrc..."
		sed -i '/###plexydesk rpmbuild/,/additions###/d' "${HOME}/.rpmrc"
	fi
	if [ -f "${HOME}/.rpmmacros" ]; then
		prog_ok "Removing the added rpm resources from ${HOME}/.rpmmacros..."
		sed -i '/###plexydesk rpmbuild/,/additions###/d' "${HOME}/.rpmmacros"
	fi
	}

# Ensure we have the rpmbuild binary
if ! eval "which rpmbuild > /dev/null 2>&1"; then
	prog_err "It seems that you do not have the 'rpmbuild' binary in your PATH. ABORTING..."
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

# We will work from the base GIT folder
cd "${GIT_DIR}"

# Ensure we are in the base folder
if ! [ -f 'AUTHORS' ]; then
	prog_err "The $(pwd) is not the base ${APP_NAME} folder. ABORTING..."
	cd "${CUR_DIR}"
	exit 1
fi

# Ensure we have all folders created
if ! [ -d "${READY_DIR}/rpm" ]; then
	prog_ok "Creating ${READY_DIR}/rpm folder..."
	mkdir -p "${READY_DIR}/rpm"
fi
if ! [ -d "${BUILD_DIR}" ]; then
	prog_ok "Creating ${BUILD_DIR} folder..."
	mkdir -p "${BUILD_DIR}"
fi

create_rpm_structures

# Check if source tarball exists and copy it to the rpm structure
if ! [ -f "${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2" ]; then
	dist/make_dist_source.sh

	if [ $? -ne 0 ]; then
		prog_err "Creating the dist RPM files at stage: dist source tarball prepare..."
		cleanup_rpmrc_additions
		cd "${CUR_DIR}"
		exit 1
	fi
fi

prog_ok "Copying the dist source tarball to $(pwd)/build/rpm/SOURCES folder..."
cp "${READY_DIR}/${APP_NAME}-${APP_VERSION}.tar.bz2" "${BUILD_DIR}/rpm/SOURCES"

prog_ok "Building the dist RPM files with rpmbuild..."
rpmbuild -ba dist/plexydesk.spec

if [ $? -ne 0 ]; then
    prog_err "Creating the dist RPM files..."
    cleanup_rpmrc_additions
	cd "${CUR_DIR}"
	exit 1
fi

cleanup_rpmrc_additions

prog_ok "Copying the dist RPM files to ${READY_DIR}/rpm folder..."
find "${BUILD_DIR}/rpm" -name "*.rpm" -exec cp "{}" "${READY_DIR}/rpm" \;

if ! [ -z "${GPGKEY}" ] ; then
	prog_ok "Signing the RPM packages with the GPG key ${GPGKEY}..."
	rpm --resign ${READY_DIR}/rpm/*.rpm
fi

prog_ok "Dist RPM files are ready."
if [ -z "${GPGKEY}" ] ; then
	prog_warn "Don't forget to sign them with 'rpm --resign *.rpm' if they will be distributed."
fi
prog_warn "You can find the dist RPM files here:\n  ${READY_DIR}/rpm\n"

ls -la "${READY_DIR}/rpm"

cd "${CUR_DIR}"
