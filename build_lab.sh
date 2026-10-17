#!/usr/bin/env bash
# Builds solar_sim without CMake or sudo, for Linux machines that only have
# g++ and git. On the first run it downloads a prebuilt raylib 5.5 and
# raygui 4.0 into ~/deps (or into $DEPS_DIR, if that's set).
#
# Usage: ./build_lab.sh, then ./solar_sim

# Print an error message and stop the script.
die() {
    echo "build_lab.sh: error: $*" >&2
    exit 1
}

# Work from the folder this script is in, so it can be run from anywhere.
cd "$(dirname "$0")" || die "could not change into the script's folder"

DEPS_DIR="${DEPS_DIR:-$HOME/deps}"
RAYLIB_DIR="$DEPS_DIR/raylib-5.5_linux_amd64"
RAYGUI_DIR="$DEPS_DIR/raygui-4.0"
RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz"

mkdir -p "$DEPS_DIR" || die "could not create $DEPS_DIR"

if [ ! -d "$RAYLIB_DIR" ]; then
    echo "Downloading raylib 5.5 into $DEPS_DIR ..."
    archive="$DEPS_DIR/raylib-5.5_linux_amd64.tar.gz"
    if command -v wget > /dev/null; then
        wget -O "$archive" "$RAYLIB_URL" || die "could not download $RAYLIB_URL"
    elif command -v curl > /dev/null; then
        curl -fL -o "$archive" "$RAYLIB_URL" || die "could not download $RAYLIB_URL"
    else
        die "neither wget nor curl is installed, so raylib can't be downloaded"
    fi
    if ! tar -xzf "$archive" -C "$DEPS_DIR"; then
        rm -rf "$RAYLIB_DIR" # don't leave a half-extracted copy for the next run to use
        die "could not extract $archive"
    fi
    rm -f "$archive"
fi

if [ ! -d "$RAYGUI_DIR" ]; then
    echo "Downloading raygui 4.0 into $DEPS_DIR ..."
    if ! git clone --depth 1 --branch 4.0 https://github.com/raysan5/raygui.git "$RAYGUI_DIR"; then
        rm -rf "$RAYGUI_DIR" # git keeps a half-made copy if only the checkout step failed
        die "could not clone raygui"
    fi
fi

echo "Compiling solar_sim ..."
g++ -std=c++20 -O2 src/main.cpp src/sim/simulation.cpp -Isrc \
    -I"$RAYLIB_DIR/include" -I"$RAYGUI_DIR/src" "$RAYLIB_DIR/lib/libraylib.a" \
    -lm -ldl -lpthread -o solar_sim \
    || die "compilation failed, see the g++ errors above"

echo "Built $PWD/solar_sim"
