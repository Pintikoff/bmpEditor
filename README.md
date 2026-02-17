# BMP Image Editor

A command-line tool for manipulating 24-bit BMP image files with various operations including rotation, mirroring, cropping, framing, and color adjustments.

## Features

- **Mirror** - Flip images horizontally or vertically
- **Rotate** - Rotate images by 90°, 180°, or 270°
- **Crop** - Extract rectangular regions from images
- **Frame** - Add colored borders around images
- **Tint** - Adjust RGB color channels
- **Zoom** - Scale images up by any factor
- **Shrink** - Reduce image size by half with quality averaging

## Requirements

- C compiler (GCC recommended)
- 24-bit BMP image files

## Compilation

### Using the compile script (recommended)

```bash
gcc compile.c -o compile
./compile
```

### Manual compilation

```bash
gcc -o bmp_editor main.c bmp_edit.c bmp_read.c bmp_other.c
```

## Usage

```bash
./bmp_editor [operation] [parameters]
```

## Operations

### Mirror

Mirror the image horizontally or vertically.

```bash
./bmp_editor -m <axis>
./bmp_editor --mirror <axis>
```

**Parameters:**
- `x` - horizontal mirror
- `y` - vertical mirror

**Example:**
```bash
./bmp_editor -m x
./bmp_editor -m y
```

---

### Rotate

Rotate the image clockwise.

```bash
./bmp_editor -r <angle>
./bmp_editor --rotate <angle>
```

**Parameters:**
- `90` - rotate 90° clockwise
- `180` - rotate 180°
- `270` - rotate 270° clockwise (90° counter-clockwise)

**Example:**
```bash
./bmp_editor -r 90
./bmp_editor -r 180
```

---

### Crop

Crop image to a specified rectangular region.

```bash
./bmp_editor -c <startX> <startY> <endX> <endY>
./bmp_editor --crop <startX> <startY> <endX> <endY>
```

**Parameters:**
- Coordinates of top-left corner (startX, startY)
- Coordinates of bottom-right corner (endX, endY)

**Example:**
```bash
./bmp_editor -c 10 10 100 100
./bmp_editor -c 0 0 50 50
```

---

### Frame

Add a colored frame around the image.

```bash
./bmp_editor -f <width> <R> <G> <B>
./bmp_editor --frame <width> <R> <G> <B>
```

**Parameters:**
- `width` - frame thickness in pixels
- `R G B` - RGB color values (0-255)

**Examples:**
```bash
./bmp_editor -f 5 255 0 0      # Red frame, 5px
./bmp_editor -f 10 0 255 0     # Green frame, 10px
./bmp_editor -f 3 0 0 255      # Blue frame, 3px
```

---

### Tint

Adjust color channels of the image.

```bash
./bmp_editor -t <color> <value>
./bmp_editor --tint <color> <value>
```

**Parameters:**
- `color` - which channel to adjust (`r`, `g`, or `b`)
- `value` - adjustment amount (-255 to +255)

**Examples:**
```bash
./bmp_editor -t r 50      # Increase red by 50
./bmp_editor -t b -30     # Decrease blue by 30
./bmp_editor -t g 100     # Increase green by 100
```

---

### Zoom

Scale the image up by a specified factor.

```bash
./bmp_editor -z [factor]
./bmp_editor --zoom [factor]
```

**Parameters:**
- `0` or no parameter - 2x zoom (default)
- `factor` - custom zoom factor (integer)

**Examples:**
```bash
./bmp_editor -z        # 2x zoom
./bmp_editor -z 3      # 3x zoom
./bmp_editor -z 4      # 4x zoom
```

---

### Shrink

Reduce image size by half using 2×2 pixel block averaging for quality.

```bash
./bmp_editor -s
./bmp_editor --shrink
```

**Parameters:** None (automatic 2x reduction)

**Example:**
```bash
./bmp_editor -s        # Shrink to 50%
```

---

### Help

Display the help message.

```bash
./bmp_editor -h
./bmp_editor --help
```

## Important Notes

- **File Format:** Only 24-bit BMP files are supported
- **Coordinate System:** (0,0) represents the top-left corner
- **BMP Storage:** Images are stored upside-down in BMP format internally
- **Output:** Processed images will be saved to the specified output directory

## Example Workflows

```bash
# Mirror an image vertically
./bmp_editor -m y

# Rotate 180 degrees
./bmp_editor -r 180

# Crop to 50x50 from top-left corner
./bmp_editor -c 0 0 50 50

# Add a green 10-pixel frame
./bmp_editor -f 10 0 255 0

# Decrease blue channel by 30
./bmp_editor -t b -30

# Zoom image 4x
./bmp_editor -z 4

# Shrink image to half size
./bmp_editor -s
```

## Project Structure

```
bmp_editor/
├── README.md
├── compile.c          # Compilation helper script
├── main.c             # Main program entry point
├── [operation files]  # Individual operation implementations
└── [header files]     # Function declarations and structures
```

## License

This project is available for educational and personal use.

## Contributing

Contributions are welcome! Please ensure code follows existing style and includes appropriate error handling.

## Author

Lev - Cybersecurity and Programming Student

---

**Note:** This is a learning project focused on understanding C programming fundamentals, memory management, and file format specifications.