#!/usr/bin/env bash

if ! ./grid-xml-test test
then
  echo grid-xml-test execution failed.
  exit 1
fi

if ! cmp test.cmp test.xml > /dev/null
then
  echo test.cmp and test.xml differ
  exit 1
fi

echo PASSED
