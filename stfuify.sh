#!/bin/sh

STFUIFY_DYLIB_DIR="$( cd "$( dirname "$0" )" && pwd )"
STFUIFY_DYLIB_PATH="$STFUIFY_DYLIB_DIR/stfuify.dylib"

if [ ! -a "$STFUIFY_DYLIB_PATH" ]; then
	make -C $STFUIFY_DYLIB_DIR
fi
echo $STFUIFY_DYLIB_PATH

export DYLD_INSERT_LIBRARIES="$STFUIFY_DYLIB_PATH"
export DYLD_FORCE_FLAT_NAMESPACE=1
if [ -z "$SPOTIFY_BINARY_PATH" ]; then
	SPOTIFY_BINARY_PATH=/Applications/Spotify.app/Contents/MacOS/Spotify
fi
$SPOTIFY_BINARY_PATH
