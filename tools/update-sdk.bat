@echo off

pushd ..

pushd deps/cpp-sdk
git pull upstream dev
pushd ../..
