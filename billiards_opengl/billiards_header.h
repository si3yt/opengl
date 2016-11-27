#pragma once

/* windows */
#define _WINDOW_POSITION_X 100;
#define _WINDOW_POSITION_Y 100;
#define _WINDOW_WIDTH 512;
#define _WINDOW_HEIGHT 512;
#define _WINDOW_TITLE "billiards";

/* light */
#define _LIGHT_POSITION_0 {0.0, 0.0, 150.0, 1.0};

/* ball */
#define _FRICTION 0.995;
#define _POS_ADJUSTMENT 0.01;
#define _VEC_THRESHOLD 0.01;
#define _BALL_RADIUS 4.0;

/* floor */
#define _FLOOR_WIDTH 100.0;
#define _FLOOR_HEIGHT 200.0;

/* wall */
#define _WALL_SIZE 10.0;

/* box */
#define _BOX_FACE { 0, 1, 2, 3, 1, 5, 6, 2, 5, 4, 7, 6, 4, 0, 3, 7, 4, 5, 1, 0, 3, 2, 6, 7};

/* get array length */
template
<
	typename TYPE,
	std::size_t SIZE
>
std::size_t array_length(const TYPE(&)[SIZE])
{
	return SIZE;
}
