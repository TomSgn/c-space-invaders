# Xcode Project Setup Guide

This guide will help you set up an Xcode project for Space Invaders on iOS.

## Quick Setup (Recommended Method)

### Step 1: Create New Xcode Project

1. Open Xcode
2. File → New → Project
3. Select "iOS" → "App"
4. Fill in details:
   - Product Name: `SpaceInvaders`
   - Team: Your development team
   - Organization Identifier: `com.yourcompany`
   - Interface: `Storyboard` (or `SwiftUI`, doesn't matter much)
   - Language: `Objective-C` (C files will work fine)
5. Save in this directory (`ios/`)

### Step 2: Add Source Files

1. In Xcode, right-click on `SpaceInvaders` group
2. Select "Add Files to SpaceInvaders..."
3. Add these files:
   - `main_ios.c`
   - All files from `../src/` directory
   - All files from `../include/` directory
4. Make sure "Copy items if needed" is **unchecked**

### Step 3: Add Assets

1. Right-click on `SpaceInvaders` group
2. "Add Files to SpaceInvaders..."
3. Navigate to `../assets/`
4. Select the entire `assets` folder
5. Check "Create folder references" (not groups)
6. Click "Add"

### Step 4: Configure Build Settings

1. Select your project in the Navigator
2. Select your target
3. Go to "Build Settings"
4. Search for "Header Search Paths"
5. Add:
   ```
   $(PROJECT_DIR)/../include
   $(PROJECT_DIR)/../src
   ```
6. Search for "Framework Search Paths"
7. Add paths to your SDL2 frameworks

### Step 5: Add SDL2 Frameworks

1. Select your target
2. Go to "General" tab
3. Scroll to "Frameworks, Libraries, and Embedded Content"
4. Click "+" and add:
   - SDL2.framework
   - SDL2_image.framework
   - SDL2_ttf.framework
5. Set embedding to "Embed & Sign"

### Step 6: Configure Info.plist

Replace the default Info.plist with the one provided in this directory, or add these keys:

```xml
<key>UIStatusBarHidden</key>
<true/>
<key>UIViewControllerBasedStatusBarAppearance</key>
<false/>
<key>UISupportedInterfaceOrientations</key>
<array>
    <string>UIInterfaceOrientationPortrait</string>
</array>
```

### Step 7: Build and Run

1. Select your device or simulator
2. Click the Play button or press ⌘+R
3. The game should compile and run!

## Manual Command-Line Build (Alternative Method)

If you prefer not to use Xcode GUI:

```bash
# Create Xcode project from command line
mkdir -p SpaceInvaders.xcodeproj
# (Then manually edit project.pbxproj file - complex)

# Or use the Makefile approach
./build_ios.sh device
```

## Common Build Issues

### Issue: "SDL.h file not found"

**Solution:**
1. Make sure SDL2 frameworks are installed
2. In Xcode Build Settings, add framework paths
3. Check "Framework Search Paths" includes SDL2 location

### Issue: "Undefined symbols for architecture arm64"

**Solution:**
1. Verify SDL2 frameworks are iOS versions (not macOS)
2. Check that all .c files are in "Compile Sources" build phase
3. Ensure frameworks are properly linked

### Issue: "Could not find or use auto-linked framework 'SDL2'"

**Solution:**
1. In Build Settings, disable "Auto-linking" for frameworks
2. Manually add frameworks as described in Step 5

### Issue: "App crashes on launch with code signing error"

**Solution:**
1. Select your target → Signing & Capabilities
2. Choose a valid team
3. Enable "Automatically manage signing"

## Deployment

### To iOS Simulator
1. Select a simulator from the device menu
2. Click Run (⌘+R)

### To Physical Device
1. Connect your iOS device via USB
2. Select it from the device menu
3. Ensure device is in Developer Mode
4. Click Run
5. Trust the developer certificate on device

## Project Structure in Xcode

Your Xcode project should look like:

```
SpaceInvaders
├── SpaceInvaders (group)
│   ├── main_ios.c
│   ├── Source Files (group)
│   │   ├── init.c
│   │   ├── update.c
│   │   ├── draw.c
│   │   └── shoot.c
│   ├── Headers (group)
│   │   ├── globals.h
│   │   ├── init.h
│   │   ├── update.h
│   │   ├── draw.h
│   │   └── shoot.h
│   ├── assets (folder reference)
│   ├── Info.plist
│   └── LaunchScreen.storyboard
├── Frameworks
│   ├── SDL2.framework
│   ├── SDL2_image.framework
│   └── SDL2_ttf.framework
└── Products
    └── SpaceInvaders.app
```

## Optimization Tips

### For Better Performance

1. **Enable Release Build**
   - Product → Scheme → Edit Scheme
   - Set Build Configuration to "Release"

2. **Compiler Optimizations**
   - Build Settings → Optimization Level
   - Set to "Fastest, Smallest [-Os]" or "Fastest [-O3]"

3. **Strip Debug Symbols**
   - Build Settings → Strip Debug Symbols During Copy
   - Set to "Yes"

### For Debugging

1. **Enable Exceptions**
   - Build Settings → Enable Exception Handling
   - Set to "Yes"

2. **Debug Symbols**
   - Build Settings → Debug Information Format
   - Set to "DWARF with dSYM File"

## Testing Checklist

- [ ] App launches without crashing
- [ ] Touch controls respond correctly
- [ ] Graphics render properly
- [ ] Game logic works (enemies spawn, bullets fire, etc.)
- [ ] Score and lives display correctly
- [ ] Shield effect works
- [ ] No memory leaks (use Instruments)
- [ ] Runs on both simulator and device
- [ ] Works on different iOS versions (13.0+)
- [ ] Portrait orientation locks correctly

## Resources

- [Xcode Documentation](https://developer.apple.com/documentation/xcode)
- [SDL2 iOS Guide](https://wiki.libsdl.org/README/iOS)
- [iOS App Distribution Guide](https://developer.apple.com/documentation/xcode/distributing-your-app-for-beta-testing-and-releases)

## Next Steps

After successful build:
1. Test on multiple devices/screen sizes
2. Add app icon (Assets.xcassets)
3. Create launch screen
4. Submit to App Store (requires Apple Developer Program)
