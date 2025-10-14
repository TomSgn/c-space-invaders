# iOS Quick Start Guide

Get Space Invaders running on your iOS device in 5 minutes!

## Prerequisites

✅ macOS computer  
✅ Xcode installed  
✅ iOS device (or use Simulator)  
✅ SDL2 iOS frameworks (download links below)

## Step 1: Get SDL2 Frameworks

Download these frameworks for iOS:

1. **SDL2**: https://github.com/libsdl-org/SDL/releases
   - Look for `SDL2-2.x.x.dmg`
   - Mount the DMG and copy `SDL2.framework` to a known location

2. **SDL2_image**: https://github.com/libsdl-org/SDL_image/releases
   - Download iOS framework

3. **SDL2_ttf**: https://github.com/libsdl-org/SDL_ttf/releases
   - Download iOS framework

Or use Homebrew to install them locally:
```bash
brew install sdl2 sdl2_image sdl2_ttf
```

## Step 2: Quick Build (Command Line)

```bash
# Navigate to the ios directory
cd ios

# Edit Makefile.ios and update framework paths:
# SDL2_PATH = /path/to/your/SDL2.framework
# SDL2_IMAGE_PATH = /path/to/your/SDL2_image.framework
# SDL2_TTF_PATH = /path/to/your/SDL2_ttf.framework

# Build for iOS device
./build_ios.sh device

# Or build for simulator
./build_ios.sh simulator
```

## Step 3: Run Using Xcode (Easiest)

### Option A: Create New Xcode Project

1. Open Xcode
2. Create new iOS App project named "SpaceInvaders"
3. Add files:
   - `main_ios.c`
   - All files from `../src/`
   - All files from `../include/`
4. Add `assets` folder as folder reference
5. Add SDL2 frameworks to project
6. Replace Info.plist with provided one
7. Build and Run!

📖 **Detailed instructions**: See [XCODE_SETUP.md](XCODE_SETUP.md)

### Option B: Use Existing Build

If you already built using command line:

```bash
# Create app bundle
mkdir -p SpaceInvaders.app
cp SpaceInvaders SpaceInvaders.app/
cp Info.plist SpaceInvaders.app/
cp -r ../assets SpaceInvaders.app/

# Sign (replace with your identity)
codesign -s "iPhone Developer" SpaceInvaders.app

# Deploy to device
ios-deploy --bundle SpaceInvaders.app
```

## Controls

Once running:

| Action | Control |
|--------|---------|
| **Move ship** | Tap and drag anywhere on screen |
| **Shoot** | Red button (bottom right) |
| **Shield** | Blue button (bottom left) |
| **Missiles** | Purple button (bottom center) |
| **Start game** | Tap screen (from menu) |

## Troubleshooting

### "Build Failed: SDL.h not found"

Edit `Makefile.ios` and set correct paths to SDL2 frameworks:

```makefile
SDL2_PATH = /usr/local/opt/SDL2  # Update this
```

### "No valid signing certificate"

In Xcode:
1. Select project → Signing & Capabilities
2. Select your team
3. Enable automatic signing

### "App crashes on launch"

Make sure:
- Assets folder is included in app bundle
- All frameworks are properly embedded
- Using iOS versions of SDL2 (not macOS versions)

### "Touch controls don't work"

Verify:
- SDL2 was built with iOS touch support
- Running on actual iOS device or simulator (not macOS)

## What's Different from Desktop?

| Desktop | iOS |
|---------|-----|
| Keyboard controls | Touch controls |
| Window mode | Fullscreen |
| Mouse optional | Touch required |
| Quit with ESC | Swipe up from bottom |

## Performance

The game runs at 60 FPS with:
- VSync enabled
- Hardware acceleration
- Optimized rendering

Perfect performance on:
- ✅ iPhone 8 and newer
- ✅ iPad (all models)
- ✅ iOS 13.0+

## Next Steps

1. 🎮 Customize controls in `main_ios.c`
2. 🎨 Add your own textures in `assets/`
3. 🔊 Add sound effects (requires SDL_mixer)
4. 📱 Submit to App Store

## Need Help?

- 📖 Full documentation: [README.md](README.md)
- 🛠️ Xcode setup: [XCODE_SETUP.md](XCODE_SETUP.md)
- 🐛 Issues: Create issue on GitHub

## Minimum Requirements

| Requirement | Version |
|-------------|---------|
| iOS | 13.0+ |
| Xcode | 12.0+ |
| SDL2 | 2.0+ |
| Device | iPhone 6s or newer |

Enjoy playing Space Invaders on iOS! 🚀
