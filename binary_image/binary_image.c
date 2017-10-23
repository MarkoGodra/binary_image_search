#include "binary_image.h"

const int_least8_t MAX_HEIGHT = 100;
const int_least8_t MAX_WIDTH = 100;

static int_least8_t get_string_size(const char str[]);
static int_least8_t check_text(FILE* fp); //Checks if input matrix contains something else than '0' or '1'
static int_least8_t check_fault(dot_t** matrix, int_least8_t x, int_least8_t y);

dot_t** load_matrix(FILE* fp, const int_least8_t width, const int_least8_t height)
{
	dot_t** matrix = NULL;
	int_least8_t i;
	int_least8_t j;
	char ch;

	rewind(fp);

	//If matrix is regulary shaped
	if((width != -1) && (height != -1))
	{
		/* Allocate memory for matrix, depending on input text size */
		matrix = (dot_t**)malloc(width * sizeof(dot_t*));
		for(i = 0; i < width; i++)
		{
			matrix[i] = (dot_t*)malloc(height * sizeof(dot_t));
		}

		i = 0;
		j = 0;
		ch = fgetc(fp);
		while(ch != EOF)
		{
			if((ch != '\n') && (ch != 0)) //If regular character put it into matrix
			{
				matrix[i][j].value = ch - '0'; //Safe conversion, standard section 5.2.1, paragraph 2/3
				matrix[i][j].visited = FALSE;
				matrix[i][j].fault = FALSE;
				i++;
			}
			else // If end of line, go to next line of matrix
			{
				j++;
				i = 0;
			}
			ch = fgetc(fp);
		}
	}

	fclose(fp);

	return matrix;
}

void free_matrix(dot_t** matrix, const int_least8_t width)
{
	int_least8_t i;
	for(i = 0; i < width; i++) // If width is invalid, (This loop won't execute)
	{
		free(matrix[i]);
	}
	free(matrix); // It will only free matrix ptr

	return;
}

int_least8_t check_matrix_size(FILE* fp, int_least8_t* width, int_least8_t* height)
{
	char row[INT_LEAST8_MAX] = {0};
	int_least8_t ret_val = TRUE;
	int_least8_t size_of_row;
	int_least32_t width_checker = 0;
	int_least32_t height_checker = 0;
	int_least8_t valid_input;

	valid_input = check_text(fp); //Checks for characters other than 1 or 0

	rewind(fp);
	char ch = fgetc(fp);

	while(ch != '\n')
	{
		width_checker++;
		ch = fgetc(fp);
	}

	rewind(fp);

	while(fgets(row, INT_LEAST8_MAX, fp))
	{
		height_checker++;
		if((size_of_row = get_string_size(row)) != width_checker)
		{
			ret_val = FALSE;
			width_checker = -1;
			height_checker = -1;
			break;
		}
	}

	//TODO: Change size of input matrix here
	if((width_checker > INT_LEAST8_MAX) || (height_checker > INT_LEAST8_MAX))
	{
		width_checker = -1;
		height_checker = -1;
		ret_val = FALSE;
	}
	else
	{
		if(valid_input == TRUE)
		{
			*height = height_checker;
			*width = width_checker;
		}
		else
		{
			*height = -1;
			*width = -1;
			ret_val = FALSE;
		}
	}

	return ret_val;
}

static int_least8_t get_string_size(const char str[]) //Retruns length of single row
{
	int_least8_t size = 0;
	const char* ptr = str;

	while((*ptr != '\n') && (*ptr != '\0'))
	{
		size++;
		ptr++;
	}

	return size;
}

int_least8_t find_longest_line(dot_t** matrix, const int_least8_t width, const int_least8_t height)
{
	int_least8_t max_line_len = 0;
	int_least8_t current_line_len = 0;
	int_least8_t x;
	int_least8_t y;
	int_least8_t done_jump = FALSE;
	queue_t* queue = create_queue(sizeof(position_t));

	for(y = 1; y < height - 1; y++)
	{
		for(x = 1; x < width - 1; x++)
		{
			int_least8_t xx = x; //Current pixel position
			int_least8_t yy = y;

			printf("(%"PRIdLEAST8", %"PRIdLEAST8") visited\n", xx, yy);

			if((matrix[x][y].value == 1) && (matrix[x][y].visited == FALSE))
			{
				printf("(%"PRIdLEAST8", %"PRIdLEAST8") point of interest\n", xx, yy);
				current_line_len = 0; //Found 1 dot, so it is a line

				//While loop, to go through entire line
				while(1)
				{
					//Check if matrix is valid
					int_least8_t valid = check_fault(matrix, xx, yy);
					if(valid == FALSE)
					{
						//If node is fault, all his ancestors in line are fault, return to them and set them to fault
						printf("(%"PRIdLEAST8", %"PRIdLEAST8") is fault\n", xx, yy);
						matrix[xx][yy].fault = TRUE;
						while((is_empty(queue)) == FALSE)
						{
							node_t node = dequeue(queue);
							position_t* pos = (position_t*)node.value;
							matrix[pos->x][pos->y].fault = TRUE;
						}
						current_line_len = 0;
						break;
					}
					printf("(%"PRIdLEAST8", %"PRIdLEAST8") is not fault\n", xx, yy);


					if(matrix[xx][yy].visited == FALSE)
					{
						printf("(%"PRIdLEAST8", %"PRIdLEAST8") pushed to queue\n", xx, yy);
						position_t pos;
						pos.x = xx;
						pos.y = yy;
						node_t* node = create_node((void*)&pos, sizeof(position_t));
						enqueue(queue, node);
						current_line_len++;
					}
					matrix[xx][yy].visited = TRUE;

					//If got top unvisited neighbrough move to him
					if((matrix[xx][yy - 1].value == 1) && (matrix[xx][yy - 1].visited == FALSE))
					{
						yy--;
						continue;
					}
					//If got right unvisited neighbrough move to him
					if((matrix[xx + 1][yy].value == 1) && (matrix[xx + 1][yy].visited == FALSE))
					{
						xx++;
						continue;
					}
					//If got bottom unvisited neigbrough move to him
					if((matrix[xx][yy + 1].value == 1) && (matrix[xx][yy + 1].visited == FALSE))
					{
						yy++;
						continue;
					}
					//If got right unvisited neighbrough move to him
					if((matrix[xx - 1][yy].value == 1) && (matrix[xx - 1][yy].visited == FALSE))
					{
						xx--;
						continue;
					}

					/*If while got to this point that means there are no unvisited neighbroughs
					 * try returning to first element and do same thing for him again */
					if((is_empty(queue)) == FALSE && (done_jump == FALSE)) //If queue is not empty (Peek at front)
					{
						/*Trying to go back to first dot (Entry point of current line)
						 * Jump back is possible to do only once, because if there
						 * is need for more, it is end of line and more than one jump would mean
						 * that line has cross
						 */
						node_t* peek_node = front(queue);
						position_t* pos = (position_t*)(peek_node->value);
						xx = pos->x;
						yy = pos->y;
						done_jump = TRUE;
						continue;
					}

					//Update longest line holder
					if(current_line_len > max_line_len)
					{
						max_line_len = current_line_len;
						current_line_len = 0;
					}

					//Empty the queue so new dots can be inserted
					empty_queue(queue);

					break;
				}
			}
		}
	}

	delete_queue(queue);
	return max_line_len;
}

static int_least8_t check_text(FILE* fp)
{
	int_least8_t ret_val = TRUE;
	char ch;
	rewind(fp);
	ch = fgetc(fp);

	while(ch != EOF)
	{
		if((ch != '0') && (ch != '1') && (ch  != '\n'))
		{
			ret_val = FALSE;
		}
		ch = fgetc(fp);
	}

	return ret_val;
}

dot_t** extend_matrix(dot_t** matrix, int_least8_t* width, int_least8_t* height)
{
	dot_t** new_matrix = NULL;
	int_least8_t i;
	int_least8_t x;
	int_least8_t y;
	int_least8_t old_width = *width;
	int_least8_t old_height = *height;

	*width += 2;
	*height += 2;

	//Allocate memory for new matrix
	new_matrix = (dot_t**)malloc(*width * sizeof(dot_t*));
	for(i = 0; i < *width; i++)
	{
		new_matrix[i] = (dot_t*)malloc(*height * sizeof(dot_t));
	}

	//Copy old matrix to new one
	for(y = 0; y < old_height; y++)
	{
		for(x = 0; x < old_width; x++)
		{
			new_matrix[x + 1][y + 1] = matrix[x][y];
		}
	}

	//Fill extended places with zeros, and set them to visited, so they won't be considered
	for(y = 0; y < *height; y++)
	{
		for(x = 0; x < *width; x++)
		{
			if((x == 0) || (y == 0) || (x == *width - 1) || (y == *height - 1)) //If it's out of old matrix
			{
				new_matrix[x][y].value = 0;
				new_matrix[x][y].visited = TRUE;
				new_matrix[x][y].fault = FALSE;
			}
		}
	}

	//Free old matrix
	for(i = 0; i < old_width; i++)
	{
		free(matrix[i]);
	}
	free(matrix);

	return new_matrix;
}

static int_least8_t check_fault(dot_t** matrix, int_least8_t x, int_least8_t y)
{
	int_least8_t ret_val = TRUE;
	int_least8_t neighbrough_count = 0;

	//Dot itself is fault if one of her neighbroughs is fault
	if(matrix[x][y - 1].fault == TRUE) //UP
	{
		ret_val = FALSE;
	}
	else if(matrix[x + 1][y].fault == TRUE) //LEFT
	{
		ret_val = FALSE;
	}
	else if(matrix[x][y + 1].fault == TRUE) //DOWN
	{
		ret_val = FALSE;
	}
	else if(matrix[x - 1][y].fault == TRUE) //RIGHT
	{
		ret_val = FALSE;
	}

	//Check all neighbrouhs (UP, LEFT, DOWN, RIGHT)
	if(matrix[x][y - 1].value == 1) //UP
	{
		neighbrough_count++;
	}

	if(matrix[x + 1][y].value == 1) //RIGHT
	{
		neighbrough_count++;
	}

	if(matrix[x][y + 1].value == 1) //DOWN
	{
		neighbrough_count++;
	}

	if(matrix[x - 1][y].value == 1) //LEFT
	{
		neighbrough_count++;
	}

	//If exactly 2 neighbroughs, check diagonaly
	if(neighbrough_count == 2)
	{
		//If there are up and right neighbroughs check top right
		if((matrix[x][y - 1].value == 1) && (matrix[x + 1][y].value == 1))
		{
			if(matrix[x + 1][y - 1].value == 1) //TOP RIGHT
			{
				neighbrough_count++;
			}
		}
		//If there are right and bottom neighbrough check bottom right
		if((matrix[x + 1][y].value == 1) && (matrix[x][y + 1].value == 1))
		{
			if(matrix[x + 1][y + 1].value == 1) //DOWN RIGHT
			{
				neighbrough_count++;
			}
		}
		//If there are left and bottom neighbrough check bottom left
		if((matrix[x - 1][y].value == 1) && (matrix[x][y + 1].value == 1))
		{
			if(matrix[x - 1][y + 1].value == 1) //DOWN LEFT
			{
				neighbrough_count++;
			}
		}
		//If there are left and top neighbrough check top left
		if((matrix[x - 1][y].value == 1) && (matrix[x][y - 1].value == 1))
		{
			if(matrix[x - 1][y - 1].value == 1) //DOWN RIGHT
			{
				neighbrough_count++;
			}
		}

	}

	//If total neighbrough count is more than 3 it's not valid
	if(neighbrough_count >= 3)
	{
		ret_val = FALSE;
	}

	return ret_val;
}