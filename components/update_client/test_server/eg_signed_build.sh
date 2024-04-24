#!/usr/bin/env bash

if [ -z ${1+x} ]; then
  BUILD_TYPE='qa';
else
  BUILD_TYPE=$1;
fi
echo $BUILD_TYPE

COBALT=~/github/cobalt
BUILD_HOME=~/tmp/signed_build/

rm -rf $BUILD_HOME/cobalt
mkdir -p $BUILD_HOME/cobalt/lib && \
mkdir -p $BUILD_HOME/cobalt/content

$COBALT/cobalt/build/gn.py -p linux-x64x11 -c $BUILD_TYPE
$COBALT/cobalt/build/gn.py -p evergreen-x64 -c $BUILD_TYPE

ninja -C $COBALT/out/evergreen-x64_$BUILD_TYPE/ cobalt_install && \
cp $COBALT/out/evergreen-x64_$BUILD_TYPE/libcobalt.so $BUILD_HOME/cobalt/lib && \
cp -r $COBALT/out/evergreen-x64_$BUILD_TYPE/content/* $BUILD_HOME/cobalt/content

cat > $BUILD_HOME/cobalt/manifest.json <<EOF
{
	"manifest_version": 2,
	"name": "Cobalt",
	"description": "Cobalt",
	"version": "1.0.0.0"
}
EOF

if [ ! -f $BUILD_HOME/cobalt.pem ]; then
  /opt/google/chrome/chrome --pack-extension=$BUILD_HOME/cobalt
else
  /opt/google/chrome/chrome --pack-extension=$BUILD_HOME/cobalt --pack-extension-key=$BUILD_HOME/cobalt.pem
fi
