#ifndef _TETRIS_FIGURES_H_
#define _TETRIS_FIGURES_H_

#define FIG_COORD_ROWS 4
#define FIG_COORD_COLS 2

// all possible figure's shape
typedef enum {
  I_SHAPE,
  J_SHAPE,
  L_SHAPE,
  O_SHAPE,
  S_SHAPE,
  T_SHAPE,
  Z_SHAPE,
  FIGURE_COUNT
} FigureShape;

// all possible rotation for figure
typedef enum {
  ROTATE_0,
  ROTATE_90,
  ROTATE_180,
  ROTATE_270,
  ROTATE_COUNT
} FigureRotation;

// allias
typedef int Figures[FIGURE_COUNT][ROTATE_COUNT][FIG_COORD_ROWS][FIG_COORD_COLS];
typedef int (*FiguresPtr)[ROTATE_COUNT][FIG_COORD_ROWS][FIG_COORD_COLS];
typedef int FigureCoord[FIG_COORD_ROWS][FIG_COORD_COLS];
typedef int (*FigureCoordPtr)[FIG_COORD_COLS];

// Figure define figure in game field
typedef struct {
  int xc, yc;
  FigureShape shape;
  FigureRotation rotation;
  FigureCoord coords;
} Figure;

// funcs
FigureShape get_random_shape();

FiguresPtr figures();
Figure copy(Figure *fig);

void update_figure_coord(Figure *fig);
void get_coord_for_shape(FigureCoordPtr fig_coord, FigureShape shape);
int check_borders(Figure *fig);

void rotate_clockwise(Figure *fig);
void move_fig(Figure *fig, int dx, int dy);

#endif
