#!/bin/sh
#
#

LCOV_ROOT=${LCOV_ROOT:=/usr}
LCOV_CMD=${LCOV_ROOT}/bin/lcov

type ${LCOV_CMD} >/dev/null 2>&1 || { echo >&2 "[ERROR]: lcov is required but it's not installed.  Aborting."; exit 1; }

FILENAME=coverage.info
OUTPUT_DIR=coverage

if [ -f ${FILENAME} ]; then
    echo "[INFO]: Removing: ${FILENAME}"
    rm ${FILENAME}
fi


echo "[INFO]: Removing: gcda"
find `pwd` -name *.gcda | xargs rm

${LCOV_CMD} --zerocounters --directory `pwd`
${LCOV_CMD} --capture --initial --directory `pwd` --output-file ${FILENAME}
ctest 
${LCOV_CMD} --no-checksum --directory `pwd` --capture --output-file ${FILENAME}
${LCOV_CMD} --remove ${FILENAME} "/usr*" -o ${FILENAME}
${LCOV_CMD} --remove ${FILENAME} "gmock*" -o ${FILENAME}
${LCOV_CMD} --remove ${FILENAME} "test*" -o ${FILENAME}

if [ -d ${OUTPUT_DIR} ]; then
    echo "[INFO]: Removing: ${OUTPUT_DIR}"
    rm -fr ${OUTPUT_DIR}
fi

${LCOV_ROOT}/bin/genhtml --highlight --legend --show-details --num-spaces 4 --demangle-cpp --output-directory ${OUTPUT_DIR} ${FILENAME}
