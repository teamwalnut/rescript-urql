#!/usr/bin/env bash

yarn build
for dir in ./examples/*; do
  cd "$dir"
  yarn
  yarn clean
  yarn build
  cd ../
done