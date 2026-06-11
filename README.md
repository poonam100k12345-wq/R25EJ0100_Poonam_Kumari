# 2D Graphics Editor in C

A complete terminal-based 2D Graphics Editor written in standard C (C99). This project was designed for university assignments and is fully compatible with GitHub Classroom and compilation via Code::Blocks GCC compiler.

## Features

- **24x80 Canvas**: Renders high-quality ASCII shape outlines on a character-grid using `*` for shapes and `_` for the background.
- **Multiple Shapes Supported**:
  1. **Line**: Renders lines of any slope using **Bresenham's Line Algorithm**.
  2. **Rectangle**: Renders rectangle borders using width and height parameters.
  3. **Triangle**: Renders triangle outlines connecting three arbitrary vertices.
  4. **Circle**: Renders circular contours using the **Midpoint Circle Algorithm**.
- **Object Management**:
  - Maintains a structured shape listing.
  - Automatically assigns unique object IDs.
  - Real-time modification and deletion of drawn objects.
  - Efficient canvas updates by clearing and redrawing active shapes when edits occur.
- **Robust Input Handling**:
  - Validates boundaries of the canvas (`[0-79]` for X, `[0-23]` for Y).
  - Sanitizes the standard input buffer to prevent infinite loops on invalid input types (e.g., entering text instead of numbers).

---

## Code Architecture

- **Data Structures**: Uses a combination of structures (`LineData`, `RectData`, `TriangleData`, `CircleData`) inside an anonymous union inside a parent `Shape` structure to store shape properties, coordinates, and types cleanly and efficiently.
- **Header Files constraint**: Follows strict project limits, using only:
  - `<stdio.h>` (Standard Input/Output)
  - `<stdlib.h>` (Memory management, `abs()` function, helper functions)
  - `<string.h>` (String manipulations)
  - `<math.h>` (Mathematical helpers)

---

## Building and Compiling

The project compiles with standard C99 flags and contains no compiler warnings or errors.

### Compiling with GCC (CLI)

Run the following command in your terminal to build the project:

```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o main.exe -lm
```

- `-std=c99`: Enforces standard C99 compatibility.
- `-Wall -Wextra -pedantic`: Enables strict warnings to ensure code cleanliness.
- `-lm`: Links the math library (required on Unix-like environments).

### Compiling with Code::Blocks

1. Open Code::Blocks.
2. Select **File -> New -> Project...** and choose **Console Application**.
3. Select **C** as the language.
4. Name the project (e.g., `2D_Graphics_Editor`) and select your folder path.
5. Replace the auto-generated `main.c` code with the provided [main.c](file:///c:/Users/poona/OneDrive/Desktop/2D_Graphics_Editor/main.c).
6. Build and run the project using **Build -> Build and Run** (or press `F9`).

---

## Interactive Menu Options

When launched, the program runs in an interactive CLI loop:
1. **Add Object**: Select shape type (Line, Rectangle, Triangle, Circle) and input valid parameters.
2. **Delete Object**: Input the unique ID of the object to erase it.
3. **Modify Object**: Select an existing shape ID and update its coordinates.
4. **Display Canvas**: Prints the current 24x80 graphics grid.
5. **List Objects**: Lists all active shapes with their properties and IDs.
6. **Exit**: Terminate program execution safely.
