# iOS Touch Controls Guide

This document explains the touch control scheme for Space Invaders on iOS.

## Control Layout

```
┌─────────────────────────────────────┐
│          Space Invaders             │
│                                     │
│                                     │
│              [TAP & DRAG]           │
│            Movement Area            │
│                                     │
│            Your Ship: 🚀            │
│                                     │
│        (Drag anywhere to move)      │
│                                     │
│                                     │
│  ┌──────┐         ┌──────┐         │
│  │SHIELD│         │SHOOT │         │
│  │ 🛡️   │  ┌────┐ │ 🔴   │         │
│  │      │  │MSIL│ │      │         │
│  │ BLUE │  │ 💜 │ │ RED  │         │
│  └──────┘  └────┘ └──────┘         │
└─────────────────────────────────────┘
```

## Button Locations

### Bottom Row (Left to Right):

1. **Shield Button** (Bottom Left)
   - Color: Blue
   - Size: 80x80 pixels
   - Position: 20px from left, 20px from bottom
   - Function: Toggle protective shield

2. **Missile Button** (Bottom Center)
   - Color: Purple
   - Size: 80x80 pixels
   - Position: Center of screen, 20px from bottom
   - Function: Fire two homing missiles

3. **Shoot Button** (Bottom Right)
   - Color: Red
   - Size: 80x80 pixels
   - Position: 20px from right, 20px from bottom
   - Function: Fire standard bullets

## Touch Gestures

### Ship Movement
```
┌─────────────────────────┐
│   Tap and hold          │
│   anywhere on screen    │
│   (outside buttons)     │
│                         │
│   Drag finger to move   │
│   ship in any direction │
│                         │
│   Ship follows finger   │
│   position              │
└─────────────────────────┘
```

**How it works:**
1. Touch and hold on the game area
2. Ship will move toward your finger
3. Drag to continuously adjust position
4. Release to stop movement

### Shooting

#### Standard Bullets (Rapid Fire)
```
┌──────┐
│SHOOT │  ← Tap repeatedly for rapid fire
│ 🔴   │
│ RED  │
└──────┘
```
- Tap the red button
- Bullets fire upward
- Can fire rapidly
- Good for multiple enemies

#### Homing Missiles
```
┌────┐
│MSIL│  ← Tap once to fire 2 missiles
│ 💜 │
└────┘
```
- Tap the purple button
- Fires two missiles at once
- Missiles track nearest enemies
- More powerful than bullets
- Leaves purple trail effect

### Shield

```
┌──────┐
│SHIELD│  ← Tap to toggle on/off
│ 🛡️   │     (Button turns green when active)
│ BLUE │
└──────┘
```
- Tap to activate/deactivate
- Protects from enemy collisions
- Blue when inactive
- Green when active
- Pulsating effect when on

## Multi-Touch Support

You can use multiple fingers simultaneously:

- ✅ Move with one finger
- ✅ Shoot with another finger
- ✅ Activate shield while moving
- ✅ Fire missiles while dodging

Example:
```
Left thumb: Hold and drag for movement
Right thumb: Tap shoot button repeatedly
```

## Visual Feedback

### Button States

**Inactive:**
```
┌──────┐
│      │ ← Semi-transparent
│      │   Solid border
└──────┘
```

**Active/Pressed:**
```
┌══════┐
║      ║ ← Brighter color
║      ║   Thicker border
└══════┘
```

### Ship Trails

As you move, the ship leaves colored trails:

- **Movement Trail**: Light blue (sky blue)
  - Appears when moving ship
  - Fades over time
  - Shows ship's path

- **Missile Trail**: Purple
  - Appears when firing missiles
  - More vibrant than movement trail

## Tips for Mobile Play

### One-Handed Mode
```
     ┌────┐
     │ 📱 │
     │    │
     │ 👍 │  Use thumb to:
     │    │  - Move (drag)
     │    │  - Shoot (tap)
     └────┘
```

### Two-Handed Mode (Recommended)
```
  👈         👉
┌────────────────┐
│  Left    Right │
│  Move    Shoot │
│               │
│  Shield Missile│
└────────────────┘
```

Left hand: Movement + Shield  
Right hand: Shooting + Missiles

## Sensitivity

The ship movement is responsive with:
- **Movement Speed**: 5 pixels per frame
- **Update Rate**: 60 FPS
- **Touch Radius**: Button touch is 80x80px
- **Dead Zone**: Ship won't jitter if finger is within 5px of target

## Comparison with Desktop

| Action | Desktop | iOS |
|--------|---------|-----|
| Move Left | ← Arrow | Drag left |
| Move Right | → Arrow | Drag right |
| Move Up | ↑ Arrow | Drag up |
| Move Down | ↓ Arrow | Drag down |
| Shoot | Space | Red button |
| Shield | E key | Blue button |
| Missiles | A key | Purple button |
| Start Game | Enter | Tap screen |

## Customization

Want to change the controls? Edit `main_ios.c`:

### Button Positions
```c
// In InitTouchButtons() function
shootButton.rect.x = WINDOW_WIDTH - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
shootButton.rect.y = WINDOW_HEIGHT - TOUCH_BUTTON_SIZE - TOUCH_MARGIN;
```

### Button Sizes
```c
#define TOUCH_BUTTON_SIZE     80  // Change this
#define TOUCH_MARGIN          20  // And this
```

### Movement Speed
```c
#define SHIP_SPEED            5   // Adjust for faster/slower movement
```

## Accessibility

- Large buttons (80x80) for easy tapping
- High contrast colors
- Visual feedback on button press
- No complex gestures required
- Can be played with reduced motion

## Practice Mode

To practice the controls:
1. Start the game
2. Focus on movement first
3. Add shooting when comfortable
4. Try combining actions
5. Experiment with shield timing

## Advanced Techniques

### Dodge and Shoot
```
Move finger in circular motion
while tapping shoot with other hand
```

### Shield Burst
```
Activate shield just before impact
(saves shield energy)
```

### Missile Timing
```
Wait for enemies to group
then fire missiles for multi-kill
```

Happy gaming! 🎮
