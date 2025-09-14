#include "../tetris.h"

FigureShape get_random_shape() { return (FigureShape)(rand() % FIGURE_COUNT); }

FiguresPtr figures() {
  static Figures figs = {
      // I_SHAPE
      {
          {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},  // 0 deg
          {{0, -1}, {0, 0}, {0, 1}, {0, 2}},  // 90 deg
          {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, // 180 deg
          {{0, -2}, {0, -1}, {0, 0}, {0, 1}}, // 270 deg
      },
      // J_SHAPE
      {
          {{-1, -1}, {-1, 0}, {0, 0}, {1, 0}}, // 0 deg
          {{0, 1}, {0, 0}, {0, -1}, {1, -1}},  // 90 deg
          {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},   // 180 deg
          {{-1, 1}, {0, 1}, {0, 0}, {0, -1}},  // 270 deg
      },
      // L_SHAPE
      {
          {{0, -1}, {0, 0}, {0, 1}, {1, 1}},   // 0 deg
          {{-1, 1}, {-1, 0}, {0, 0}, {1, 0}},  // 90 deg
          {{-1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 180 deg
          {{-1, 0}, {0, 0}, {1, 0}, {1, -1}},  // 270 deg
      },
      // O_SHAPE
      {
          {{0, 0}, {0, 1}, {1, 1}, {1, 0}}, // 0 deg
          {{0, 0}, {0, 1}, {1, 1}, {1, 0}}, // 90 deg
          {{0, 0}, {0, 1}, {1, 1}, {1, 0}}, // 180 deg
          {{0, 0}, {0, 1}, {1, 1}, {1, 0}}, // 270 deg
      },
      // S_SHAPE
      {
          {{-1, 0}, {0, 0}, {0, -1}, {1, -1}}, // 0 deg
          {{0, -1}, {0, 0}, {1, 0}, {1, 1}},   // 90 deg
          {{-1, 1}, {0, 1}, {0, 0}, {1, 0}},   // 180 deg
          {{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}, // 270 deg
      },
      // T_SHAPE
      {
          {{-1, 0}, {0, 0}, {0, -1}, {1, 0}}, // 0 deg
          {{0, -1}, {0, 0}, {0, 1}, {1, 0}},  // 90 deg
          {{-1, 0}, {0, 0}, {0, 1}, {1, 0}},  // 180 deg
          {{-1, 0}, {0, -1}, {0, 0}, {0, 1}}, // 270 deg
      },
      // Z_SHAPE
      {
          {{-1, -1}, {0, -1}, {0, 0}, {1, 0}}, // 0 deg
          {{0, 1}, {0, 0}, {1, 0}, {1, -1}},   // 90 deg
          {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},   // 180 deg
          {{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}, // 270 deg
      },
  };

  return figs;
}

Figure copy(Figure *fig) {
  Figure copy_fig;

  copy_fig.xc = fig->xc;
  copy_fig.yc = fig->yc;

  copy_fig.shape = fig->shape;
  copy_fig.rotation = fig->rotation;

  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    copy_fig.coords[i][0] = fig->coords[i][0];
    copy_fig.coords[i][1] = fig->coords[i][1];
  }

  return copy_fig;
}

void update_figure_coord(Figure *fig) {
  FiguresPtr figs = figures();
  FigureCoordPtr fig_base_coord = figs[fig->shape][fig->rotation];

  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    fig->coords[i][0] = fig->xc + fig_base_coord[i][0];
    fig->coords[i][1] = fig->yc + fig_base_coord[i][1];
  }
}

int check_borders(Figure *fig) {
  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    if (fig->coords[i][0] < 0 || fig->coords[i][0] >= X_SIZE) {
      return CHECK_BORDER_FAIL;
    } else if (fig->coords[i][1] >= Y_SIZE) {
      return CHECK_BORDER_FAIL;
    }
  }

  return CHECK_BORDER_OK;
}

void rotate_clockwise(Figure *fig) {
  fig->rotation = (fig->rotation + 1) % ROTATE_COUNT;
  update_figure_coord(fig);
}

void move_fig(Figure *fig, int dx, int dy) {
  fig->xc += dx;
  fig->yc += dy;
  update_figure_coord(fig);
}

void get_coord_for_shape(FigureCoordPtr fig_coord, FigureShape shape) {
  FiguresPtr figs = figures();
  FigureCoordPtr fig_base_coord = figs[shape][ROTATE_0];

  for (int i = 0; i < FIG_COORD_ROWS; i++) {
    fig_coord[i][0] = fig_base_coord[i][0];
    fig_coord[i][1] = fig_base_coord[i][1];
  }
}
