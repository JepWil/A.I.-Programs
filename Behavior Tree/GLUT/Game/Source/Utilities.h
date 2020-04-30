#pragma once
#include <math.h>

struct vec2
{
	int x;
	int y;
};

static float euclideandistance(int arg_x1, int arg_y1, int arg_x2, int arg_y2)
{
	return (float)sqrt((((arg_x2 - arg_x1)*(arg_x2 - arg_x1)) + ((arg_y2 - arg_y1)*(arg_y2 - arg_y1))));
}

static float euclideandistance(vec2 p_pos1, vec2 p_pos2)
{
	return (float)sqrt((((p_pos2.x - p_pos1.x)*(p_pos2.x - p_pos1.x)) + ((p_pos2.y - p_pos1.y)*(p_pos2.y - p_pos1.y))));
}