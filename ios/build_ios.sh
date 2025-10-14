#!/bin/bash

# Build script for iOS version of Space Invaders
# This script helps automate the iOS build process

echo "==================================="
echo "Space Invaders - iOS Build Script"
echo "==================================="
echo ""

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "Error: This script must be run on macOS with Xcode installed"
    exit 1
fi

# Check if Xcode is installed
if ! command -v xcodebuild &> /dev/null; then
    echo "Error: Xcode is not installed or xcodebuild is not in PATH"
    exit 1
fi

# Parse command line arguments
BUILD_TYPE="device"
if [ "$1" == "simulator" ]; then
    BUILD_TYPE="simulator"
    echo "Building for iOS Simulator..."
elif [ "$1" == "device" ] || [ "$1" == "" ]; then
    BUILD_TYPE="device"
    echo "Building for iOS Device..."
else
    echo "Usage: $0 [device|simulator]"
    exit 1
fi

# Navigate to iOS directory
cd "$(dirname "$0")"

# Clean previous builds
echo "Cleaning previous builds..."
make -f Makefile.ios clean

# Build
echo "Building..."
if [ "$BUILD_TYPE" == "simulator" ]; then
    make -f Makefile.ios simulator
else
    make -f Makefile.ios all
fi

if [ $? -eq 0 ]; then
    echo ""
    echo "==================================="
    echo "Build successful!"
    echo "==================================="
    echo ""
    echo "Next steps:"
    echo "1. Copy the executable and assets to an iOS app bundle"
    echo "2. Sign the app with your development certificate"
    echo "3. Deploy to your iOS device or simulator"
    echo ""
    echo "Note: You'll need SDL2 frameworks for iOS."
    echo "Visit https://www.libsdl.org/download-2.0.php for iOS frameworks"
else
    echo ""
    echo "==================================="
    echo "Build failed!"
    echo "==================================="
    echo ""
    echo "Common issues:"
    echo "- SDL2 frameworks not found"
    echo "- Incorrect SDK paths"
    echo "- Missing iOS development tools"
    exit 1
fi
