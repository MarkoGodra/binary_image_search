#ifndef __BINARY_IMAGE_H__
#define __BINARY_IMAGE_H__

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "queue.h"

extern const int_least8_t MAX_WIDTH;
extern const int_least8_t MAX_HEIGHT;

typedef struct
{
	int_least8_t value;
	int_least8_t visited;
	int_least8_t fault;
}dot_t;

typedef struct //Required for saving elements position in queue
{
	int_least8_t x;
	int_least8_t y;
}position_t;

dot_t** load_matrix(FILE* fp, const int_least8_t width, const int_least8_t height);
void free_matrix(dot_t** matrix, const int_least8_t width);
int_least8_t check_matrix_size(FILE* fp, int_least8_t* width, int_least8_t* height); //Returns size of input text (width and height)
int_least8_t find_longest_line(dot_t** matrix, const int_least8_t width, const int_least8_t height);
dot_t** extend_matrix(dot_t** matrix, int_least8_t* width, int_least8_t* height);

#endif
