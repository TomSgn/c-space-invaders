# Space Invaders in C

A classic Space Invaders game implementation in C using SDL2, with support for both desktop and iOS platforms.

## Features

- Classic space shooter gameplay
- Multiple enemy types with different textures
- Bullet and missile weapons
- Shield system for protection
- Particle trail effects
- Score tracking and lives system
- Starfield background
- Game states: Menu, Game, Game Over

## Platforms

### Desktop (macOS, Linux, Windows)
- Keyboard controls
- Window mode
- Standard SDL2 rendering

### iOS (iPhone/iPad)
- Touch controls with on-screen buttons
- Portrait orientation
- Optimized for mobile devices
- See [ios/README.md](ios/README.md) for iOS-specific instructions

## Requirements

### Desktop
- GCC or Clang compiler
- SDL2
- SDL2_image
- SDL2_ttf
- pkg-config (for dependency management)

### iOS
- macOS with Xcode
- SDL2 frameworks for iOS
- iOS SDK 13.0 or later
- See [ios/README.md](ios/README.md) for complete requirements

## Installation

### macOS (Desktop version)

```bash
# Install dependencies using Homebrew
brew install sdl2 sdl2_image sdl2_ttf

# Build
make

# Run
./space_invaders
```

### Linux (Desktop version)

```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

# Fedora
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel

# Build
make

# Run
./space_invaders
```

### iOS

See detailed instructions in [ios/README.md](ios/README.md)

```bash
cd ios
./build_ios.sh device
```

## Controls

### Desktop (Keyboard)
- **Arrow Keys**: Move ship (Left/Right/Up/Down)
- **Space**: Shoot bullets
- **E**: Toggle shield
- **A**: Fire missiles
- **Enter**: Start game (from menu/game over)
- **Escape/Close Window**: Quit game

### iOS (Touch)
- **Tap and Drag**: Move ship
- **Red Button** (bottom right): Shoot bullets
- **Blue Button** (bottom left): Toggle shield
- **Purple Button** (bottom center): Fire missiles
- **Tap Screen**: Start game (from menu/game over)

## Project Structure

```
c-space-invaders/
├── main.c              # Desktop version main file
├── Makefile            # Desktop build configuration
├── include/            # Header files
│   ├── globals.h       # Global definitions and structs
│   ├── init.h          # Initialization functions
│   ├── update.h        # Game logic updates
│   ├── draw.h          # Rendering functions
│   └── shoot.h         # Shooting mechanics
├── src/                # Source files
│   ├── init.c          # Initialization implementations
│   ├── update.c        # Game logic implementations
│   ├── draw.c          # Rendering implementations
│   └── shoot.c         # Shooting implementations
├── assets/             # Game assets
│   ├── 2X/             # Ship and enemy textures
│   └── arial.ttf       # Font file
└── ios/                # iOS-specific files
    ├── main_ios.c      # iOS version with touch controls
    ├── Info.plist      # iOS app configuration
    ├── Makefile.ios    # iOS build configuration
    ├── build_ios.sh    # iOS build script
    └── README.md       # iOS-specific documentation
```

## Building

### Desktop

```bash
# Build
make

# Clean
make clean

# Rebuild
make re
```

### iOS

```bash
cd ios

# Build for device
./build_ios.sh device

# Build for simulator
./build_ios.sh simulator

# Or use make directly
make -f Makefile.ios
```

## Game Mechanics

### Weapons
- **Bullets**: Standard rapid-fire weapon (Space key / Red button)
- **Missiles**: Homing missiles that track nearest enemy (A key / Purple button)

### Defense
- **Shield**: Temporarily protects ship from enemy collisions (E key / Blue button)

### Scoring
- Destroy enemies to increase score
- Higher score increases enemy spawn rate
- Difficulty scales with score:
  - 0-50: Normal spawn rate (2000ms)
  - 50-100: Faster spawn (1500ms)
  - 100-200: Fast spawn (1000ms)
  - 200+: Very fast spawn (700ms)

## Development

### Code Style
- C11 standard
- Modular structure with separate files for different systems
- Global state management via globals.h

### Adding New Features
1. Define structures in `include/globals.h`
2. Implement logic in appropriate source file (`src/`)
3. Add function declarations to corresponding header
4. Update both `main.c` and `ios/main_ios.c` for cross-platform support

## Assets

The game uses:
- PNG textures for ships (17 variants for player + enemies)
- TrueType font (Arial) for text rendering
- Procedurally generated:
  - Starfield background
  - Particle trails
  - Bullets and missiles

## Known Issues

- Asset paths are relative; run from project root directory
- Font and textures are optional but recommended for best experience

## Future Improvements

- [ ] Sound effects and music
- [ ] More enemy types and patterns
- [ ] Power-ups system
- [ ] High score persistence
- [ ] Android support
- [ ] Landscape orientation for tablets

## Contributing

Contributions are welcome! Please ensure:
1. Code compiles on both desktop and iOS
2. Follow existing code style
3. Test on multiple platforms when possible
4. Update relevant README files

## Credits

- SDL2 library: https://www.libsdl.org/
- Game assets: [Specify source/author]

## License

[Specify your license here]
