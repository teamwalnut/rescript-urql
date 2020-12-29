#!/usr/bin/env bash

yarn build
for dir in GITHUB_WORKSPACE/examples/*; do
  cd "$dir"
  yarn
  yarn clean
  yarn build
  cd ../
done