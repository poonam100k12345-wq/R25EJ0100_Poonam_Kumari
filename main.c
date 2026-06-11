/**
 * -----------------------------------------------------------------------------
 * 2D Graphics Editor in C
 * 
 * Course Assignment Project
 * Standard: C99
 * Allowed headers: stdio.h, stdlib.h, string.h, math.h
 * Canvas dimensions: 24 rows x 80 columns
 * Character encoding: '*' for shapes, '_' for background
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Global Constants */
#define CANVAS_ROWS 24
#define CANVAS_COLS 80
#define MAX_SHAPES 100

/* Enumeration for Shape Types */
typedef enum {
    SHAPE_LINE = 1,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
    SHAPE_CIRCLE
} ShapeType;

/* Structure definition for a Line */
typedef struct {
    int x1, y1;
    int x2, y2;
} LineData;

/* Structure definition for a Rectangle */
typedef struct {
    int x, y;
    int width, height;
} RectData;

/* Structure definition for a Triangle */
typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleData;

/* Structure definition for a Circle */
typedef struct {
    int xc, yc;
    int radius;
} CircleData;

/* General Shape Structure storing type and coordinates */
typedef struct {
    int id;
    ShapeType type;
    union {
        LineData line;
        RectData rect;
        TriangleData triangle;
        CircleData circle;
    } data;
} Shape;

/* Function Prototypes */
void clear_canvas(char canvas[CANVAS_ROWS][CANVAS_COLS]);
void displayCanvas(const char canvas[CANVAS_ROWS][CANVAS_COLS]);
void plot_pixel(char canvas[CANVAS_ROWS][CANVAS_COLS], int x, int y);
int is_valid_point(int x, int y);

/* Shape Drawing functions */
void draw_line(char canvas[CANVAS_ROWS][CANVAS_COLS], int x1, int y1, int x2, int y2);
void draw_rectangle(char canvas[CANVAS_ROWS][CANVAS_COLS], int x, int y, int width, int height);
void draw_triangle(char canvas[CANVAS_ROWS][CANVAS_COLS], int x1, int y1, int x2, int y2, int x3, int y3);
void draw_circle(char canvas[CANVAS_ROWS][CANVAS_COLS], int xc, int yc, int radius);
static void plot_circle_points(char canvas[CANVAS_ROWS][CANVAS_COLS], int xc, int yc, int x, int y);

/* Object list management functions */
void list_shapes(const Shape shapes[], int count);
void redraw_all(char canvas[CANVAS_ROWS][CANVAS_COLS], const Shape shapes[], int count);
int delete_shape(Shape shapes[], int *count, int id);
int find_shape_index(const Shape shapes[], int count, int id);

/* User input verification helpers */
int get_valid_int(const char *prompt, int min_val, int max_val);

int main(void) {
    char canvas[CANVAS_ROWS][CANVAS_COLS];
    Shape shapes[MAX_SHAPES];
    int shape_count = 0;
    int next_id = 1;
    int choice = 0;

    /* Initialize the canvas */
    clear_canvas(canvas);

    printf("====================================================\n");
    printf("          2D Graphics Editor (C99 Standard)          \n");
    printf("====================================================\n");

    /* Main application loop */
    while (choice != 6) {
        printf("\n--- Main Menu ---\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Canvas\n");
        printf("5. List Objects\n");
        printf("6. Exit\n");
        
        choice = get_valid_int("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: { /* Add Object */
                if (shape_count >= MAX_SHAPES) {
                    printf("Error: Object list is full (maximum is %d).\n", MAX_SHAPES);
                    break;
                }
                
                printf("\nSelect Shape Type:\n");
                printf("1. Line\n");
                printf("2. Rectangle\n");
                printf("3. Triangle\n");
                printf("4. Circle\n");
                int type_choice = get_valid_int("Enter type (1-4): ", 1, 4);

                Shape new_shape;
                new_shape.id = next_id;
                new_shape.type = (ShapeType)type_choice;

                if (type_choice == 1) { /* Line */
                    printf("\nEnter line endpoints:\n");
                    new_shape.data.line.x1 = get_valid_int("Start X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.line.y1 = get_valid_int("Start Y (0-23): ", 0, CANVAS_ROWS - 1);
                    new_shape.data.line.x2 = get_valid_int("End X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.line.y2 = get_valid_int("End Y (0-23): ", 0, CANVAS_ROWS - 1);
                } 
                else if (type_choice == 2) { /* Rectangle */
                    printf("\nEnter top-left corner coordinates and dimensions:\n");
                    new_shape.data.rect.x = get_valid_int("Top-Left X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.rect.y = get_valid_int("Top-Left Y (0-23): ", 0, CANVAS_ROWS - 1);
                    /* Max width and height are constrained by the size of the canvas starting from top-left coordinates */
                    int max_w = CANVAS_COLS - new_shape.data.rect.x;
                    int max_h = CANVAS_ROWS - new_shape.data.rect.y;
                    new_shape.data.rect.width = get_valid_int("Width (1 to fit): ", 1, max_w);
                    new_shape.data.rect.height = get_valid_int("Height (1 to fit): ", 1, max_h);
                } 
                else if (type_choice == 3) { /* Triangle */
                    printf("\nEnter coordinates for all three vertices:\n");
                    new_shape.data.triangle.x1 = get_valid_int("Vertex 1 X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.triangle.y1 = get_valid_int("Vertex 1 Y (0-23): ", 0, CANVAS_ROWS - 1);
                    new_shape.data.triangle.x2 = get_valid_int("Vertex 2 X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.triangle.y2 = get_valid_int("Vertex 2 Y (0-23): ", 0, CANVAS_ROWS - 1);
                    new_shape.data.triangle.x3 = get_valid_int("Vertex 3 X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.triangle.y3 = get_valid_int("Vertex 3 Y (0-23): ", 0, CANVAS_ROWS - 1);
                } 
                else if (type_choice == 4) { /* Circle */
                    printf("\nEnter center coordinates and radius:\n");
                    new_shape.data.circle.xc = get_valid_int("Center X (0-79): ", 0, CANVAS_COLS - 1);
                    new_shape.data.circle.yc = get_valid_int("Center Y (0-23): ", 0, CANVAS_ROWS - 1);
                    
                    /* The circle radius should be positive; warn if it clips but allow clipping with boundary checking */
                    new_shape.data.circle.radius = get_valid_int("Radius (0-80): ", 0, CANVAS_COLS);
                }

                /* Add shape to our array list */
                shapes[shape_count] = new_shape;
                shape_count++;
                next_id++;

                /* Redraw and update canvas */
                redraw_all(canvas, shapes, shape_count);
                printf("\nObject added successfully with ID: %d\n", new_shape.id);
                break;
            }

            case 2: { /* Delete Object */
                if (shape_count == 0) {
                    printf("\nNo objects to delete.\n");
                    break;
                }
                list_shapes(shapes, shape_count);
                int del_id = get_valid_int("Enter the ID of the object to delete: ", 1, next_id - 1);

                if (delete_shape(shapes, &shape_count, del_id)) {
                    redraw_all(canvas, shapes, shape_count);
                    printf("\nObject ID %d deleted successfully.\n", del_id);
                } else {
                    printf("\nError: Object ID %d not found.\n", del_id);
                }
                break;
            }

            case 3: { /* Modify Object */
                if (shape_count == 0) {
                    printf("\nNo objects to modify.\n");
                    break;
                }
                list_shapes(shapes, shape_count);
                int mod_id = get_valid_int("Enter the ID of the object to modify: ", 1, next_id - 1);
                int index = find_shape_index(shapes, shape_count, mod_id);

                if (index == -1) {
                    printf("\nError: Object ID %d not found.\n", mod_id);
                    break;
                }

                printf("\nModifying Object ID %d (%s):\n", mod_id, 
                       (shapes[index].type == SHAPE_LINE) ? "Line" :
                       (shapes[index].type == SHAPE_RECTANGLE) ? "Rectangle" :
                       (shapes[index].type == SHAPE_TRIANGLE) ? "Triangle" : "Circle");

                if (shapes[index].type == SHAPE_LINE) {
                    shapes[index].data.line.x1 = get_valid_int("New Start X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.line.y1 = get_valid_int("New Start Y (0-23): ", 0, CANVAS_ROWS - 1);
                    shapes[index].data.line.x2 = get_valid_int("New End X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.line.y2 = get_valid_int("New End Y (0-23): ", 0, CANVAS_ROWS - 1);
                }
                else if (shapes[index].type == SHAPE_RECTANGLE) {
                    shapes[index].data.rect.x = get_valid_int("New Top-Left X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.rect.y = get_valid_int("New Top-Left Y (0-23): ", 0, CANVAS_ROWS - 1);
                    int max_w = CANVAS_COLS - shapes[index].data.rect.x;
                    int max_h = CANVAS_ROWS - shapes[index].data.rect.y;
                    shapes[index].data.rect.width = get_valid_int("New Width (1 to fit): ", 1, max_w);
                    shapes[index].data.rect.height = get_valid_int("New Height (1 to fit): ", 1, max_h);
                }
                else if (shapes[index].type == SHAPE_TRIANGLE) {
                    shapes[index].data.triangle.x1 = get_valid_int("New Vertex 1 X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.triangle.y1 = get_valid_int("New Vertex 1 Y (0-23): ", 0, CANVAS_ROWS - 1);
                    shapes[index].data.triangle.x2 = get_valid_int("New Vertex 2 X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.triangle.y2 = get_valid_int("New Vertex 2 Y (0-23): ", 0, CANVAS_ROWS - 1);
                    shapes[index].data.triangle.x3 = get_valid_int("New Vertex 3 X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.triangle.y3 = get_valid_int("New Vertex 3 Y (0-23): ", 0, CANVAS_ROWS - 1);
                }
                else if (shapes[index].type == SHAPE_CIRCLE) {
                    shapes[index].data.circle.xc = get_valid_int("New Center X (0-79): ", 0, CANVAS_COLS - 1);
                    shapes[index].data.circle.yc = get_valid_int("New Center Y (0-23): ", 0, CANVAS_ROWS - 1);
                    shapes[index].data.circle.radius = get_valid_int("New Radius (0-80): ", 0, CANVAS_COLS);
                }

                /* Redraw with the updated parameters */
                redraw_all(canvas, shapes, shape_count);
                printf("\nObject ID %d modified successfully.\n", mod_id);
                break;
            }

            case 4: { /* Display Canvas */
                printf("\n--- Canvas Render (24x80) ---\n");
                displayCanvas(canvas);
                printf("-----------------------------\n");
                break;
            }

            case 5: { /* List Objects */
                list_shapes(shapes, shape_count);
                break;
            }

            case 6: { /* Exit */
                printf("\nExiting editor. Goodbye!\n");
                break;
            }
        }
    }

    return 0;
}

/**
 * Fills the entire canvas with the default background character ('_').
 */
void clear_canvas(char canvas[CANVAS_ROWS][CANVAS_COLS]) {
    for (int r = 0; r < CANVAS_ROWS; r++) {
        for (int c = 0; c < CANVAS_COLS; c++) {
            canvas[r][c] = '_';
        }
    }
}

/**
 * Outputs the canvas contents to the console row by row.
 */
void displayCanvas(const char canvas[CANVAS_ROWS][CANVAS_COLS]) {
    for (int r = 0; r < CANVAS_ROWS; r++) {
        for (int c = 0; c < CANVAS_COLS; c++) {
            putchar(canvas[r][c]);
        }
        putchar('\n');
    }
}

/**
 * Validates whether the point (x, y) resides inside the 24x80 array bounds.
 */
int is_valid_point(int x, int y) {
    return (x >= 0 && x < CANVAS_COLS && y >= 0 && y < CANVAS_ROWS);
}

/**
 * Safe pixel writing routine.
 * Enforces boundary checking and plots the foreground pixel ('*') on the canvas.
 */
void plot_pixel(char canvas[CANVAS_ROWS][CANVAS_COLS], int x, int y) {
    if (is_valid_point(x, y)) {
        canvas[y][x] = '*';
    }
}

/**
 * Draws a straight line using Bresenham's Line Algorithm.
 * Works accurately for all quadrants and slopes using integer arithmetic.
 */
void draw_line(char canvas[CANVAS_ROWS][CANVAS_COLS], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot_pixel(canvas, x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

/**
 * Draws a rectangle outline with the specified top-left corner and dimensions.
 */
void draw_rectangle(char canvas[CANVAS_ROWS][CANVAS_COLS], int x, int y, int width, int height) {
    /* Plot horizontal top and bottom segments */
    for (int i = 0; i < width; i++) {
        plot_pixel(canvas, x + i, y);
        plot_pixel(canvas, x + i, y + height - 1);
    }
    /* Plot vertical left and right segments */
    for (int i = 0; i < height; i++) {
        plot_pixel(canvas, x, y + i);
        plot_pixel(canvas, x + width - 1, y + i);
    }
}

/**
 * Draws a triangle outline by drawing lines between its three vertices.
 */
void draw_triangle(char canvas[CANVAS_ROWS][CANVAS_COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}

/**
 * Helper to plot symmetric circle coordinates across all 8 octants.
 */
static void plot_circle_points(char canvas[CANVAS_ROWS][CANVAS_COLS], int xc, int yc, int x, int y) {
    plot_pixel(canvas, xc + x, yc + y);
    plot_pixel(canvas, xc - x, yc + y);
    plot_pixel(canvas, xc + x, yc - y);
    plot_pixel(canvas, xc - x, yc - y);
    plot_pixel(canvas, xc + y, yc + x);
    plot_pixel(canvas, xc - y, yc + x);
    plot_pixel(canvas, xc + y, yc - x);
    plot_pixel(canvas, xc - y, yc - x);
}

/**
 * Draws a circle outline using the Midpoint Circle Algorithm.
 */
void draw_circle(char canvas[CANVAS_ROWS][CANVAS_COLS], int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    plot_circle_points(canvas, xc, yc, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        plot_circle_points(canvas, xc, yc, x, y);
    }
}

/**
 * Displays a formatted list of all active graphical shapes currently drawn.
 */
void list_shapes(const Shape shapes[], int count) {
    if (count == 0) {
        printf("\nNo objects created yet.\n");
        return;
    }
    printf("\n================================= ACTIVE OBJECTS LIST =================================\n");
    printf("%-5s | %-12s | %-55s\n", "ID", "Shape Type", "Details & Coordinates");
    printf("---------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("%-5d | %-12s | Endpoints: (%d, %d) to (%d, %d)\n",
                       shapes[i].id, "Line",
                       shapes[i].data.line.x1, shapes[i].data.line.y1,
                       shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("%-5d | %-12s | Top-Left: (%d, %d), Width: %d, Height: %d\n",
                       shapes[i].id, "Rectangle",
                       shapes[i].data.rect.x, shapes[i].data.rect.y,
                       shapes[i].data.rect.width, shapes[i].data.rect.height);
                break;
            case SHAPE_TRIANGLE:
                printf("%-5d | %-12s | Vertices: (%d, %d), (%d, %d), (%d, %d)\n",
                       shapes[i].id, "Triangle",
                       shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                       shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                       shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
            case SHAPE_CIRCLE:
                printf("%-5d | %-12s | Center: (%d, %d), Radius: %d\n",
                       shapes[i].id, "Circle",
                       shapes[i].data.circle.xc, shapes[i].data.circle.yc,
                       shapes[i].data.circle.radius);
                break;
        }
    }
    printf("================================================---------------------------------------\n");
}

/**
 * Resets canvas to background and draws all shapes stored in the object list.
 */
void redraw_all(char canvas[CANVAS_ROWS][CANVAS_COLS], const Shape shapes[], int count) {
    clear_canvas(canvas);
    for (int i = 0; i < count; i++) {
        switch (shapes[i].type) {
            case SHAPE_LINE:
                draw_line(canvas, shapes[i].data.line.x1, shapes[i].data.line.y1,
                                  shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(canvas, shapes[i].data.rect.x, shapes[i].data.rect.y,
                                       shapes[i].data.rect.width, shapes[i].data.rect.height);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(canvas, shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                                      shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                                      shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
            case SHAPE_CIRCLE:
                draw_circle(canvas, shapes[i].data.circle.xc, shapes[i].data.circle.yc,
                                    shapes[i].data.circle.radius);
                break;
        }
    }
}

/**
 * Deletes an object by unique ID from the active list.
 * Shifts remaining shapes in memory to keep the list contiguous.
 * Returns 1 on success, 0 if ID is not found.
 */
int delete_shape(Shape shapes[], int *count, int id) {
    int index = -1;
    for (int i = 0; i < *count; i++) {
        if (shapes[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 0;
    }
    for (int i = index; i < *count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    (*count)--;
    return 1;
}

/**
 * Searches the shapes list for the target ID.
 * Returns shape index if found, or -1 if not found.
 */
int find_shape_index(const Shape shapes[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (shapes[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * Helper to prompt user for an integer, perform input stream cleansing,
 * and enforce a minimum and maximum range validation.
 */
int get_valid_int(const char *prompt, int min_val, int max_val) {
    int val;
    char term;
    while (1) {
        printf("%s", prompt);
        /* 
         * Verify we read an integer followed by a whitespace character/newline,
         * to prevent partial input scanning like "12abc" being parsed as "12".
         */
        if (scanf("%d%c", &val, &term) == 2 && (term == '\n' || term == ' ' || term == '\t')) {
            if (val >= min_val && val <= max_val) {
                return val;
            } else {
                printf("Error: Value must be between %d and %d.\n", min_val, max_val);
            }
        } else {
            printf("Error: Invalid input. Please enter an integer.\n");
            /* Clear the stdin input buffer up to the next newline */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}
