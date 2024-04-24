#!/usr/bin/env bash

if [ -z ${1+x} ]; then
  BUILD_TYPE='qa';
else
  BUILD_TYPE=$1;
fi
echo $BUILD_TYPE


COBALT=~/github/cobalt
DEMO_HOME=~/coeg_demo

rm -rf ~/.cobalt_storage/*

rm -rf $DEMO_HOME
mkdir -p $DEMO_HOME/content/app/cobalt/lib && \
mkdir -p $DEMO_HOME/content/app/cobalt/content

$COBALT/cobalt/build/gn.py -p linux-x64x11 -c $BUILD_TYPE
$COBALT/cobalt/build/gn.py -p evergreen-x64 -c $BUILD_TYPE

ninja -C $COBALT/out/linux-x64x11_$BUILD_TYPE/ loader_app_install && \
cp $COBALT/out/linux-x64x11_$BUILD_TYPE/loader_app $DEMO_HOME && \
cp -r $COBALT/out/linux-x64x11_$BUILD_TYPE/content/* $DEMO_HOME/content

ninja -C $COBALT/out/evergreen-x64_$BUILD_TYPE/ cobalt_install && \
cp $COBALT/out/evergreen-x64_$BUILD_TYPE/libcobalt.so $DEMO_HOME/content/app/cobalt/lib && \
cp -r $COBALT/out/evergreen-x64_$BUILD_TYPE/content/* $DEMO_HOME/content/app/cobalt/content

ninja -C $COBALT/out/linux-x64x11_$BUILD_TYPE/ native_target/crashpad_handler && \
cp $COBALT/out/linux-x64x11_$BUILD_TYPE/native_target/crashpad_handler $DEMO_HOME

cat > $DEMO_HOME/content/app/cobalt/manifest.json <<EOF
{
	"manifest_version": 2,
	"name": "Cobalt",
	"description": "Cobalt",
	"version": "1.0.0.0"
}
EOF
