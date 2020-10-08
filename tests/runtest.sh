#!/bin/bash

TESTSDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
TESTID="${1}"

INFILE="${TESTSDIR}/in${TESTID}.cpp"
OUTFILE="${TESTSDIR}/out${TESTID}.md"

if [[ "${TESTID}" == "" ]]; then
  echo "usage: ./runtest.sh <id> [-r]"
  echo "   ex: ./runtest.sh 001"
  echo "  opt: -r replaces current reference"
  exit 1
fi

if [[ ! -e "${INFILE}" ]]; then
  echo "invalid test id (${TESTID}) - cannot find ${INFILE}"
  exit 1
fi

if [[ ! -e "${OUTFILE}" ]]; then
  echo "invalid test id (${TESTID}) - cannot find ${OUTFILE}"
  exit 1
fi

RV="0"
SRCTMP="$(mktemp -d)"
OUTTMP="$(mktemp -d)"
REFTMP="$(mktemp -d)"

cp -a ${INFILE} ${SRCTMP}/
cp -a ${OUTFILE} ${REFTMP}/

./bin/doxygenmd ${SRCTMP} ${OUTTMP}/out${TESTID}.md

diff -r ${REFTMP} ${OUTTMP} > /dev/null
if [[ "${?}" != "0" ]]; then
  if [[ "${2}" == "-r" ]]; then
    echo "replacing reference for ${TESTID}"
    cp ${OUTTMP}/out${TESTID}.md ${OUTFILE}
    RV="0"
  else
    echo "test failed (${TESTID}), diff:"
    diff -r ${REFTMP} ${OUTTMP}
    RV="1"
  fi
else
  echo "test passed (${TESTID})"
  RV="0"
fi

exit ${RV}

