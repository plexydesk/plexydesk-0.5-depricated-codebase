#!/bin/bash

# This script modifies all the needed files from the GIT tree
# for a release of PlexyDesk

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

confirm() {
	printf "${color_warn}%s (Y)es/(N)o? [%s]${color_normal}  " "$1" "$2"
	unset user_input
	typeset -g -l confirm_answer="$2"
	read user_input
	if [ "$user_input" != "" ]; then
		confirm_answer="$user_input"
	fi
	}

check_app_version() {
	# Check the current app version that is in CMakeLists.txt
	export APP_NAME="$( grep -e '\W*SET\W*(\W*APPLICATION_EXE_NAME' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*APPLICATION_EXE_NAME\W*(\w*)\W*\)/\1/' )"

	APP_VERSION_MAJOR="$( grep -e '\W*SET\W*(\W*LIB_MAJOR' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_MAJOR\W*(\w*)\W*\)/\1/' )"
	APP_VERSION_MINOR="$( grep -e '\W*SET\W*(\W*LIB_MINOR' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_MINOR\W*(\w*)\W*\)/\1/' )"
	APP_VERSION_RELEASE="$( grep -e '\W*SET\W*(\W*LIB_RELEASE' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*LIB_RELEASE\W*(\w*)\W*\)/\1/' )"
	export APP_VERSION="${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_RELEASE}"

	APP_DEBUG="$( grep -e '\W*SET\W*(\W*CMAKE_BUILD_TYPE' ${GIT_DIR}/CMakeLists.txt | sed -r 's/^\W*SET\W*CMAKE_BUILD_TYPE\W*(\w*)\W*$/\1/' )"
	}

get_version_input() {
	printf "${color_warn}\t%s of ${APP_NAME}? [%s]$color_normal:  " "$1" "$2"
	unset user_input
	typeset -g -i answer="$2"
	read user_input
	if [ "$user_input" != "" ]; then
		answer="$user_input"
	fi
	}

get_debug_input() {
	printf "${color_warn}\tType of the release of ${APP_NAME}?\n\tTypes: (N)one, (D)ebug, (R)elease, RelWithDeb(I)nfo, (M)inSizeRel. [%s]$color_normal:  " "$1"
	unset user_input
	typeset -g -l answer_debug="$1"
	read user_input
	if [ "$user_input" != "" ]; then
		answer_debug="$user_input"
	fi
	}

test_build_install(){
	# Check if we have cmake and make installed
	if ! eval "which cmake > /dev/null 2>&1"; then
		prog_err "cmake binarry MISSING. Cannot test building. Skipping the test..."
		return 0
	fi
	if ! eval "which make > /dev/null 2>&1"; then
		prog_err "make binarry MISSING. Cannot test building. Skipping the test..."
		return 0
	fi

	# Ensure we have all folders created
	TEST_BUILD_DIR="${BUILD_DIR}/test-build"
	if ! [ -d "${TEST_BUILD_DIR}" ]; then
		prog_ok "Creating ${TEST_BUILD_DIR} folder..."
		mkdir -p "${TEST_BUILD_DIR}"
	fi
	cd "${BUILD_DIR}"

	prog_ok "Testing 'configure' stage of the build..."
	cmake ../ -DCMAKE_INSTALL_PREFIX="${TEST_BUILD_DIR}/usr"
	if [ $? -ne 0 ]; then
		prog_err "'configure' stage problem...\nPlease fix this first. NO CHANGES TO TREE MADE..."
		cd "${CUR_DIR}"
		exit 1
	fi

	prog_ok "Testing 'make' stage of the build..."
	make
	if [ $? -ne 0 ]; then
		prog_err "'make' stage problem...\nPlease fix this first. NO CHANGES TO TREE MADE..."
		cd "${CUR_DIR}"
		exit 1
	fi

	prog_ok "Testing 'make install' stage of the build..."
	make install
	if [ $? -ne 0 ]; then
		prog_err "'make install' stage problem...\nPlease fix this first. NO CHANGES TO TREE MADE..."
		cd "${CUR_DIR}"
		exit 1
	fi

	prog_ok "Testing 'make uninstall' stage of the build..."
	make uninstall
	if [ $? -ne 0 ]; then
		prog_err "'make uninstall' stage problem...\nPlease fix this first. NO CHANGES TO TREE MADE..."
		cd "${CUR_DIR}"
		exit 1
	fi

	prog_ok "Cleaning up the test build folder... (${TEST_BUILD_DIR})"
	rm -fR "${TEST_BUILD_DIR}"

	prog_ok "All build tests successful..."

	# Ensure we go back to base GIT folder
	cd "${GIT_DIR}"
	}

# Ensure we have a console text editor set
TEXT_EDITOR=$(which editor)
if ! eval "which editor > /dev/null 2>&1"; then
	prog_warn "It seems that you do not have a console text editor set..."
	if eval "which vi > /dev/null 2>&1"; then
		prog_ok "Found ${color_warn}vi${color_ok}..."
		TEXT_EDITOR=$(which vi)
	elif  eval "which nano > /dev/null 2>&1"; then
		prog_ok "Found ${color_warn}nano${color_ok}..."
		TEXT_EDITOR=$(which nano)
	else
		prog_err "No console text editor found (editor, vi or nano). ABORTING..."
		exit 1
	fi
fi
prog_ok "Console text editor set to: ${color_warn}${TEXT_EDITOR}${color_ok}..."


PACKAGES_PREPARED=0
_DEBFULLNAME="PhobosK"
_DEBEMAIL="phobosk@kbfx.net"
_GPGKEY=8EDDA157
DATE_APP_VERSION="$(LC_ALL=C date '+%Y%m%d')"
DATE_APP="$(LC_ALL=C date '+%Y-%m-%d')"
DATE_CHANGELOG="$(LC_ALL=C date '+%d %b %Y')"
DATE_RPM="$(LC_ALL=C date '+%a %b %d %Y')"
DATE_DEB="$(LC_ALL=C date '+%a, %d %b %Y %H:%M:%S %z')"

# Ensure we have a packager name, mail and a GPGKEY needed for changelogs and signing
if [ -z "${DEBFULLNAME}" ] ; then
	prog_warn "The shell variable DEBFULLNAME is not set. Using the built-in one... (${_DEBFULLNAME})"
	export DEBFULLNAME="${_DEBFULLNAME}"
fi
if [ -z "${DEBEMAIL}" ] ; then
	prog_warn "The shell variable DEBEMAIL is not set. Using the built-in one... (${_DEBEMAIL})"
	export DEBEMAIL="${_DEBEMAIL}"
fi
if [ -z "${GPGKEY}" ] ; then
	prog_warn "The shell variable GPGKEY is not set. Using the built-in one... (${_GPGKEY})"
	export GPGKEY="${_GPGKEY}"
fi

CUR_DIR=`pwd`
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
GIT_DIR="$( dirname "${SCRIPT_DIR}" )"

export BUILD_DIR="${GIT_DIR}/build"
export READY_DIR="${GIT_DIR}/INSTALLERS"

# We will work from the base GIT folder
cd "${GIT_DIR}"

# Ensure we are in the GIT repo
if ! [ -d '.git' ]; then
	prog_err "The $(pwd) is not a GIT folder.\nThis script runs only in a GIT base folder. ABORTING..."
	cd "${CUR_DIR}"
	exit 1
fi

check_app_version

# Get the new version from the user
while :
do
	prog_ok "CURRENT ${APP_NAME} is version: ---> ${APP_VERSION} <--- and is set to: ---> ${APP_DEBUG} <---"
	get_version_input "VERSION_MAJOR" "${APP_VERSION_MAJOR}"
	APP_VERSION_MAJOR="$answer"
	get_version_input "VERSION_MINOR" "${APP_VERSION_MINOR}"
	APP_VERSION_MINOR="$answer"
	get_version_input "VERSION_RELEASE" "${APP_VERSION_RELEASE}"
	APP_VERSION_RELEASE="$answer"
	APP_VERSION="${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_RELEASE}"

	get_debug_input "${APP_DEBUG}"
	if [ "$answer_debug" = "n" -o "$answer_debug" = "none" ]; then
		APP_DEBUG="None"
		APP_DEBUG_BOOL=0
	elif  [ "$answer_debug" = "d" -o "$answer_debug" = "debug" ]; then
		APP_DEBUG="Debug"
		APP_DEBUG_BOOL=1
	elif  [ "$answer_debug" = "i" -o "$answer_debug" = "relwithdebinfo" ]; then
		APP_DEBUG="RelWithDebInfo"
		APP_DEBUG_BOOL=1
	elif  [ "$answer" = "m" -o "$answer_debug" = "minsizerel" ]; then
		APP_DEBUG="MinSizeRel"
		APP_DEBUG_BOOL=0
	else
		APP_DEBUG="Release"
		APP_DEBUG_BOOL=0
	fi

	printf "\n${color_warn}${APP_NAME} that will be prepared will be version: ---> ${APP_VERSION} <--- and will be: ---> ${APP_DEBUG} <---\nIs this OK (q to quit)? [Y]:${color_normal}  "
	unset user_input
	read user_input
	if [ "$user_input" = "" -o "$user_input" = "y" -o "$user_input" = "Y" ]; then
		break
	elif [ "$user_input" = "q" -o "$user_input" = "Q" -o "$user_input" = "quit" ]; then
		prog_err "User requested QUIT. NO modifications made."
		cd "${CUR_DIR}"
		exit
	fi
done

# Test if PlexyDesk builds ok?
confirm "Shall i test if ${APP_NAME} builds without errors" "Y"
if [ "$confirm_answer" = "y" -o "$confirm_answer" = "yes" ]; then
	test_build_install

	if [ $? -ne 0 ]; then
		prog_err "Testing ${APP_NAME} build proccess..."
		cd "${CUR_DIR}"
		exit 1
	fi
fi

# Apply changes to CMakeLists.txt
prog_ok "Applying the required changes in ${GIT_DIR}/CMakeLists.txt..."
sed -i -r "s/^\W*(SET\W*LIB_MAJOR\W*)\w*(\W*\).*)/\1${APP_VERSION_MAJOR}\2/" "${GIT_DIR}/CMakeLists.txt"
sed -i -r "s/^\W*(SET\W*LIB_MINOR\W*)\w*(\W*\).*)/\1${APP_VERSION_MINOR}\2/" "${GIT_DIR}/CMakeLists.txt"
sed -i -r "s/^\W*(SET\W*LIB_RELEASE\W*)\w*(\W*\).*)/\1${APP_VERSION_RELEASE}\2/" "${GIT_DIR}/CMakeLists.txt"
sed -i -r "s/^(\W*SET\W*CMAKE_BUILD_TYPE\W*)\w*(.*)/\1${APP_DEBUG}\2/" "${GIT_DIR}/CMakeLists.txt"
sed -i -r "s/^\W*(SET\W*APPLICATION_INTERNAL_VERSION\W*)\w*(\W*\).*)/\1${DATE_APP_VERSION}\2/" "${GIT_DIR}/CMakeLists.txt"
sed -i -r "s/^\W*(SET\W*APPLICATION_DATE\W*)\w*-\w*-\w*(\W*\).*)/\1${DATE_APP}\2/" "${GIT_DIR}/CMakeLists.txt"

# Apply changes to the RPM spec file
prog_ok "Applying the required changes in ${GIT_DIR}/dist/${APP_NAME}.spec..."
sed -i -r "s/^\W*(%define\W*version\W*)\w*\.\w*\.\w*(.*)/\1${APP_VERSION}\2/" "${GIT_DIR}/dist/${APP_NAME}.spec"
sed -i -r "s/^\W*(%define\W*debug_on\W*)\w*(.*)/\1${APP_DEBUG_BOOL}\2/" "${GIT_DIR}/dist/${APP_NAME}.spec"

# Get a new entry for the ChangeLog from the user, using a text editor
printf "\n${color_warn}Press any key to add a ${APP_NAME} changelog entry...${color_normal}"
read user_input
TMP_FILE=$(mktemp)

cat <<END >>"${TMP_FILE}"
${APP_NAME}-${APP_VERSION}    ${DATE_CHANGELOG}

  * DELETE ME - PUT HERE ALL CHANGES CONCERNING ${APP_NAME}
  * Fixes: PLEASE CHANGE ME
  * Adds: PLEASE CHANGE ME
  * Changes: PLEASE CHANGE ME
  * New upstream version


END
cat "${GIT_DIR}/ChangeLog" >>"${TMP_FILE}"

${TEXT_EDITOR} +3 "${TMP_FILE}"
if [ $? -ne 0 ]; then
	prog_err "Creating the ${APP_NAME} changelog entry..."
	cd "${CUR_DIR}"
	exit 1
fi
prog_ok "Applying the changes to ${GIT_DIR}/ChangeLog..."
mv -f "${TMP_FILE}" "${GIT_DIR}/ChangeLog"


# Get a new entry for the Ubuntu/Debian changelog from the user, using a text editor
printf "\n${color_warn}Press any key to add an Ubuntu/Debian changelog entry...${color_normal}"
read user_input
TMP_FILE=$(mktemp)

cat <<END >>"${TMP_FILE}"
${APP_NAME} (${APP_VERSION}) unstable; urgency=low

  * DELETE ME - PUT HERE ONLY CHANGES CONCERNING THE DEB PACKAGES
  * Fixes: PLEASE CHANGE ME
  * Adds: PLEASE CHANGE ME
  * Changes: PLEASE CHANGE ME
  * New upstream version

 -- ${DEBFULLNAME} <${DEBEMAIL}>  ${DATE_DEB}

END
cat "${GIT_DIR}/debian/changelog" >>"${TMP_FILE}"

${TEXT_EDITOR} +3 "${TMP_FILE}"
if [ $? -ne 0 ]; then
	prog_err "Creating the Ubuntu/Debian changelog entry..."
	cd "${CUR_DIR}"
	exit 1
fi
prog_ok "Applying the changes to ${GIT_DIR}/debian/changelog..."
mv -f "${TMP_FILE}" "${GIT_DIR}/debian/changelog"

# Get a new entry for the RPM changelog from the user, using a text editor
printf "\n${color_warn}Press any key to add a RPM changelog entry...${color_normal}"
read user_input
TMP_FILE=$(mktemp)
TMP_FILE_FULL=$(mktemp)

cat <<END >>"${TMP_FILE}"
* ${DATE_RPM} ${APP_VERSION}

 - DELETE ME - PUT HERE ONLY CHANGES CONCERNING THE RPM PACKAGES
 - Fixes: PLEASE CHANGE ME
 - Adds: PLEASE CHANGE ME
 - Changes: PLEASE CHANGE ME
 - New upstream version
END
LINE_NUM=$( grep -n "^%changelog$" ${GIT_DIR}/dist/${APP_NAME}.spec | sed -r "s/^(.*):.*/\1/" )
let LINE_NUM++
tail -n +${LINE_NUM} "${GIT_DIR}/dist/${APP_NAME}.spec" >>"${TMP_FILE}"

${TEXT_EDITOR} +3 "${TMP_FILE}"
if [ $? -ne 0 ]; then
	prog_err "Creating the RPM changelog entry..."
	cd "${CUR_DIR}"
	exit 1
fi
prog_ok "Applying the changes to ${GIT_DIR}/dist/${APP_NAME}.spec..."
head -n ${LINE_NUM} "${GIT_DIR}/dist/${APP_NAME}.spec" >"${TMP_FILE_FULL}"
cat "${TMP_FILE}" >>"${TMP_FILE_FULL}"
mv -f "${TMP_FILE_FULL}" "${GIT_DIR}/dist/${APP_NAME}.spec"
rm -f "${TMP_FILE}"


# Prepare source tarball?
confirm "Shall i prepare a ${APP_NAME} source tarball" "Y"
if [ "$confirm_answer" = "y" -o "$confirm_answer" = "yes" ]; then
	PACKAGES_PREPARED=1
	dist/make_dist_source.sh

	if [ $? -ne 0 ]; then
		prog_err "Creating the dist source tarball..."
		cd "${CUR_DIR}"
		exit 1
	fi
fi

# If on a RPM supported distro, prepare RPM packs?
if [ -e /etc/mandrake-release -o -e /etc/SuSE-release -o -e /etc/fedora-release -o -e /etc/pclinuxos-release ]; then
	confirm "Shall i prepare a ${APP_NAME} dist RPMs packages" "Y"
	if [ "$confirm_answer" = "y" -o "$confirm_answer" = "yes" ]; then
		PACKAGES_PREPARED=1
		dist/make_dist_rpm.sh

		if [ $? -ne 0 ]; then
			prog_err "Creating the dist RPM packages..."
			cd "${CUR_DIR}"
			exit 1
		fi
	fi
fi

# If on Debian/Ubuntu, prepare DEB source packs?
if [ -e /etc/debian_version ]; then
	confirm "Shall i prepare a ${APP_NAME} dist Debian source packages" "Y"
	if [ "$confirm_answer" = "y" -o "$confirm_answer" = "yes" ]; then
		PACKAGES_PREPARED=1
		dist/make_dist_deb.sh

		if [ $? -ne 0 ]; then
			prog_err "Creating the dist Debian source packages..."
			cd "${CUR_DIR}"
			exit 1
		fi
	fi
fi

echo -e "\n\n"
if [ "$PACKAGES_PREPARED" = "1" ]; then
	prog_ok "All Dist files are ready. You can find them here:"
	prog_warn "  ${READY_DIR}\n"

	ls -la ${READY_DIR}/*
fi

echo -e "\n"
prog_ok "All needed changes are done successfully."
prog_ok "Please do not forget to do these things after the script exits:"
prog_warn "git add the changes"
prog_warn "git commit the changes"
prog_warn "git tag the changes with the TAG: ${color_ok}${APP_VERSION}${color_warn}"
prog_warn "git push the changes to remote"

if [ "$PACKAGES_PREPARED" = "1" ]; then
	prog_ok "Also use the produced source tarball as only tarball for the release."
	prog_warn "Otherwise the different source tarballs will differ in their checksums for the different distributions."
fi

echo -e "\n"

cd "${CUR_DIR}"
