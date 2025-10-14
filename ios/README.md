# Space Invaders - iOS Port

This directory contains the iOS adaptation of the Space Invaders game written in C using SDL2.

## Features

- **Touch Controls**: Intuitive touch-based gameplay designed for iOS devices
  - Tap and drag anywhere to move your ship
  - Red button (bottom right): Shoot bullets
  - Blue button (bottom left): Toggle shield
  - Purple button (bottom center): Fire missiles
  
- **Optimized for Mobile**: 
  - Portrait orientation support
  - Fullscreen gameplay
  - Touch-friendly UI elements
  - Efficient rendering with VSync

## Requirements

### Development Environment
- macOS with Xcode 12.0 or later
- iOS SDK 13.0 or later
- Command Line Tools for Xcode

### Dependencies
You need SDL2 frameworks compiled for iOS:
- SDL2.framework (iOS version)
- SDL2_image.framework (iOS version)
- SDL2_ttf.framework (iOS version)

## Getting SDL2 for iOS

### Option 1: Download Pre-built Frameworks
1. Visit [SDL2 Downloads](https://www.libsdl.org/download-2.0.php)
2. Download "SDL2-2.x.x.dmg" (includes iOS framework)
3. Visit [SDL2_image Downloads](https://www.libsdl.org/projects/SDL_image/)
4. Visit [SDL2_ttf Downloads](https://www.libsdl.org/projects/SDL_ttf/)

### Option 2: Build from Source
```bash
# Clone SDL2 repositories
git clone https://github.com/libsdl-org/SDL.git
git clone https://github.com/libsdl-org/SDL_image.git
git clone https://github.com/libsdl-org/SDL_ttf.git

# Build for iOS (each repository has Xcode projects in Xcode-iOS/)
```

### Important: SDL2 Framework Header Structure

This project uses the standard SDL2 include syntax (`#include <SDL2/SDL.h>`), which requires that your iOS SDL2 frameworks have headers organized with the SDL2 subdirectory:

```
SDL2.framework/Headers/SDL2/SDL.h
SDL2_image.framework/Headers/SDL2/SDL_image.h
SDL2_ttf.framework/Headers/SDL2/SDL_ttf.h
```

If your frameworks have headers at `SDL2.framework/Headers/SDL.h` (without the SDL2 subdirectory), you have two options:

1. **Create symlinks** (recommended):
   ```bash
   # Navigate to each framework's Headers directory and create a symlink
   # This makes SDL.h accessible as SDL2/SDL.h by creating a self-referencing link
   cd SDL2.framework/Headers
   ln -s . SDL2         # Creates Headers/SDL2 -> Headers
   
   cd ../../SDL2_image.framework/Headers
   ln -s . SDL2
   
   cd ../../SDL2_ttf.framework/Headers
   ln -s . SDL2
   ```
   After this, `SDL2/SDL.h` will resolve to `Headers/SDL2/SDL.h` -> `Headers/SDL.h`

2. **Modify Makefile.ios** to change include paths (not recommended as it diverges from standard SDL2 usage)

Most modern SDL2 iOS framework builds already include the SDL2 subdirectory structure to match the desktop/Linux convention.

## Building

### Using the Build Script (Recommended)

```bash
# Build for iOS device
cd ios
./build_ios.sh device

# Build for iOS Simulator
./build_ios.sh simulator
```

### Using Make Directly

```bash
cd ios

# Build for device
make -f Makefile.ios

# Build for simulator
make -f Makefile.ios simulator

# Clean build
make -f Makefile.ios clean
```

## Project Setup

Before building, you need to configure the SDL2 framework paths in `Makefile.ios`:

```makefile
SDL2_PATH = /path/to/SDL2.framework
SDL2_IMAGE_PATH = /path/to/SDL2_image.framework
SDL2_TTF_PATH = /path/to/SDL2_ttf.framework
```

## Creating an iOS App Bundle

After building, you need to create an app bundle structure:

```bash
# Create app bundle structure
mkdir -p SpaceInvaders.app

# Copy executable
cp SpaceInvaders SpaceInvaders.app/

# Copy Info.plist
cp Info.plist SpaceInvaders.app/

# Copy assets
cp -r ../assets SpaceInvaders.app/

# Sign the app (requires Apple Developer Account)
codesign -s "iPhone Developer" SpaceInvaders.app
```

## Deployment

### iOS Simulator
```bash
# Install to simulator
xcrun simctl install booted SpaceInvaders.app

# Launch
xcrun simctl launch booted com.yourcompany.spaceinvaders
```

### iOS Device
1. Connect your iOS device
2. Use Xcode to install and run:
   ```bash
   # Deploy using ios-deploy (install via: brew install ios-deploy)
   ios-deploy --bundle SpaceInvaders.app
   ```

## Differences from Desktop Version

### Controls
- **Desktop**: Keyboard (Arrow keys, Space, E, A)
- **iOS**: Touch controls with on-screen buttons

### Input Handling
- Touch events replace keyboard events
- Ship follows finger/touch position
- On-screen buttons for actions

### Display
- Optimized for portrait mobile screens
- Touch button overlays during gameplay
- Fullscreen mode with high DPI support

## File Structure

```
ios/
├── Info.plist          # iOS app configuration
├── main_ios.c          # iOS-specific main with touch controls
├── Makefile.ios        # iOS build configuration
├── build_ios.sh        # Build automation script
└── README.md           # This file
```

## Troubleshooting

### Build Errors

**"SDL.h not found"** or **"SDL2/SDL.h not found"**
- Check that SDL2 frameworks are installed
- Verify paths in Makefile.ios
- The code uses conditional compilation: iOS builds include `<SDL.h>` (framework style), while desktop builds include `<SDL2/SDL.h>` (pkg-config style)
- The Makefile.ios automatically defines `TARGET_OS_IPHONE=1` to enable iOS-specific includes

**"No architectures to compile for"**
- Ensure you're building on macOS
- Check Xcode installation: `xcodebuild -version`

**"Undefined symbols for architecture arm64"**
- Make sure SDL2 frameworks are built for iOS (not macOS)
- Check that all framework paths are correct

### Runtime Issues

**App crashes on launch**
- Verify assets are copied to app bundle
- Check that fonts and images exist in assets/

**Touch controls not working**
- Ensure SDL2 was built with iOS touch support
- Check that window has SDL_WINDOW_ALLOW_HIGHDPI flag

## Performance Tips

1. **VSync**: Already enabled via SDL_RENDERER_PRESENTVSYNC for smooth 60 FPS
2. **High DPI**: Enable with SDL_WINDOW_ALLOW_HIGHDPI for Retina displays
3. **Asset Optimization**: Use compressed PNG assets for faster loading

## Contributing

When modifying the iOS version:
1. Keep touch controls intuitive
2. Test on both iPhone and iPad
3. Ensure compatibility with iOS 13+
4. Maintain feature parity with desktop version

## License

Same license as the main Space Invaders project.

## Resources

- [SDL2 iOS Documentation](https://wiki.libsdl.org/README/iOS)
- [iOS Game Development with SDL2](https://lazyfoo.net/tutorials/SDL/)
- [Apple Developer Documentation](https://developer.apple.com/documentation/)
