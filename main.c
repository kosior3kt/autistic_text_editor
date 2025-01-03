// raylib
#include "raylib.h"

// my stuff
#include "data_structure.h"

// system stuff
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>

//TODO: clean these macros later on

// defines
int CHAR_HEIGHT = 30;
int CHAR_WIDTH	= 16;

int START_X		= 0;
int START_Y		= 0;

int SCREEN_WIDTH  = 1200;
int SCREEN_HEIGHT = 810;

int MAX_NUM_OF_CHARS = 75;
int MAX_NUM_OF_LINES = 27;

//int MAX_NUM_OF_CHARS = (SCREEN_WIDTH/CHAR_WIDTH);
//int MAX_NUM_OF_LINES = (SCREEN_HEIGHT/CHAR_HEIGHT);

// structures
typedef struct
{
	int row;
	int col;

	int font_size;
}cursor_ctx_t;


//TODO: think about this later. This seems crap
typedef struct
{
	int row;
	int col;

	int row_size;
	int col_size;
}drawing_ctx_t;


// -------- coordinate helper methods
//TODO: check this variables later - I have no fucking clue if they work or not
static int row_to_coord(int _row)
{
	return START_X + _row * CHAR_HEIGHT;
}

static int col_to_coord(int _col)
{
	return START_Y + _col * CHAR_WIDTH;
}

int last_col = 0;
int last_row = 0;
static void draw_cursor(const cursor_ctx_t* _ctx,
						const drawing_ctx_t* _drawing)
{

	int local_row = _ctx->row - _drawing->row;
	int local_col = _ctx->col - _drawing->col;

	DrawRectangle(col_to_coord(local_row),
				  row_to_coord(local_col),
				  CHAR_WIDTH, CHAR_HEIGHT - 3,
				  GREEN);

	if(last_col != _ctx->col || last_row != _ctx->row)
	{
		last_col = _ctx->col;
		last_row = _ctx->row;
		printf("drawing cursor - row: %d, col %d\n", _ctx->row, _ctx->col);
		printf("drawing cursor (normalised) - row: %d, col %d\n", local_row, local_col);
	}
}

static void render_char(Font _font, const char _glyph, int _row, int _col)
{
	if(_glyph == '\0') return;
	Vector2 temp_pos;
	temp_pos.y = row_to_coord(_row);
	temp_pos.x = col_to_coord(_col);

	//for now just ascii values as ints. lol
	DrawTextCodepoint(_font, (int)_glyph, temp_pos, 30, RAYWHITE);
}


static void render_data_structure(Font _font,
								  const data_structure_t* _data,
								  const unsigned _start_row,
								  const unsigned _start_col)
{
	unsigned row_range = _data->current_number_of_lines + _start_row;
	if(_data->current_number_of_lines + _start_row > MAX_NUM_OF_LINES)
	{
		row_range = MAX_NUM_OF_LINES;
	}
	//for(int i = _start_row; i < _data->current_number_of_lines; ++i)
	for(int i = _start_row; i < _start_row + row_range; ++i)
	{
		unsigned col_range = _data->lines[i].current + _start_col;
		if(_data->lines[i].current + _start_col > MAX_NUM_OF_CHARS)
		{
			col_range = MAX_NUM_OF_CHARS;
		}

		for(int j = _start_col; j < _start_col + col_range; ++j)
		{
			if(j > _data->lines[i].current) break;
			render_char(_font, _data->lines[i].data[j],
						i - _start_row, j - _start_col);
		}
	}
}

static void blink_cursor(const cursor_ctx_t* _ctx,
						 const drawing_ctx_t* _drawing)
{
	// TODO: make it a fucntion that switches draw and not draw for 500ms at time
	// then draw every tiem cursor moves
	const double time_d = GetTime() * 1000;
	const int time = (int)time_d;

	if(time % 1000 > 500)
	{
		draw_cursor(_ctx, _drawing);
	}
}

static void process_key_press(cursor_ctx_t* _ctx, data_structure_t* _ds)
{
#define print_key printf("col: %d, row: %d\n",_ctx->col ,_ctx->row);
	//TODO: change it all into a fucking switch
	int key_pressed = GetKeyPressed();
	if(key_pressed == 0)
	{
		WaitTime(0.01);
		return;
	}

	switch(key_pressed)
	{
		case KEY_ENTER: //-----------------------------------------------------
		{

			ds_split_lines(_ds, _ctx->col, _ctx->row);
			++_ctx->col;

			_ctx->row = 0;
			print_key
			break;
		}
		case KEY_BACKSPACE: //-------------------------------------------------
		{
			if(_ctx->row > 0)
			{
				--_ctx->row;
				ds_remove_char(_ds, _ctx->col, _ctx->row);
			}
			else if(_ctx->col > 0)
			{
				bool change_row = true;
				if(_ctx->row < _ds->lines[_ctx->col].current)
				{
					_ctx->row = _ds->lines[_ctx->col - 1].current;
					change_row = false;
				}
				--_ctx->col;
				ds_merge_lines(_ds, _ctx->col);

				//MAYBE: wrap it into function later?
				//  _ctx.row = ds_get_end_of_line(col_index);
				if(change_row)
				{
					_ctx->row = _ds->lines[_ctx->col].current;
				}
			}

			print_key
			break;
		}
		case KEY_LEFT: //------------------------------------------------------
		{
			if(_ctx->row > 0)
			{
				--_ctx->row;
			}
			print_key
			break;
		}
		case KEY_RIGHT: //-----------------------------------------------------
		{
			if(_ctx->row < _ds->lines[_ctx->col].current)
			{
				++_ctx->row;
			}
			print_key
			break;
		}
		case KEY_UP: //--------------------------------------------------------
		{
			if(_ctx->col > 0)
			{
				--_ctx->col;
				if(_ctx->row > _ds->lines[_ctx->col].current)
				{
					_ctx->row = _ds->lines[_ctx->col].current;
				}
			}
			print_key
			break;
		}
		case KEY_DOWN: //------------------------------------------------------
		{
			if(_ctx->col < _ds->current_number_of_lines - 1)
			{
				++_ctx->col;
				if(_ctx->row > _ds->lines[_ctx->col].current)
				{
					_ctx->row = _ds->lines[_ctx->col].current;
				}
			}
			print_key
			break;
		}
		case KEY_EQUAL: //-----------------------------------------------------
		{
			FILE* file_2 = fopen("/Users/jk/Programming/C/raylib/success.txt", "w");
			ds_save_to_file(_ds, &file_2);
			fclose(file_2);
			break;
		}
		case KEY_TAB: //-------------------------------------------------------
		{
			for(int i = 0; i < 4; ++i)
			{
				ds_insert_char(_ds, _ctx->col, _ctx->row, ' ');
				++_ctx->row;
			}
			print_key
			break;
		}
		default: //------------------------------------------------------------
		{
			// this is how we get the magic keys from this mess
			// useful for shortcuts I guess
			int fancy_key_pressed = key_pressed;
			key_pressed = GetCharPressed();

			if(IsKeyDown(KEY_LEFT_CONTROL))
			{
				printf("ctrl pressed");
				printf("key_pressed: %d\n", key_pressed);
				if(IsKeyDown(69))
				{
					_ctx->row = _ds->lines[_ctx->col].current;
					return;
				}
				else if(IsKeyDown(65))
				{
					_ctx->row = 0;
					return;
				}
				else if(IsKeyDown(85))
				{
					_ctx->col = 0;
					return;
				}
				else if(IsKeyDown(68))
				{
					_ctx->col = _ds->current_number_of_lines;
					return;
				}
			}

			printf("fancy inserted: %d\n", fancy_key_pressed);

			// key_pressed = 0 means shift
			if(key_pressed == 0) break;
			printf("regular inserted: %d\n", key_pressed);
			printf("=============\n");

			ds_insert_char(_ds, _ctx->col, _ctx->row, (char)key_pressed);

			++_ctx->row;
			print_key
			break;
		}
	}
}

void normalise_drawing(const cursor_ctx_t* _cursor,
					   drawing_ctx_t* _drawing)
{
	if(_cursor->row > MAX_NUM_OF_CHARS + _drawing->row - 1)
	{
		_drawing->row = _cursor->row - MAX_NUM_OF_CHARS;
	}
	else if(_cursor->row < _drawing->row)
	{
		_drawing->row = _cursor->row;
	}
	else if(_cursor->col > MAX_NUM_OF_LINES + _drawing->col - 1)
	{
		_drawing->col = _cursor->col - MAX_NUM_OF_LINES;
	}
	else if(_cursor->col < _drawing->col)
	{
		_drawing->col = _cursor->col;
	}
}


void recalculateIfResized()
{
	if(IsWindowResized())
	{
		SCREEN_WIDTH = GetScreenWidth();
		SCREEN_HEIGHT = GetScreenHeight();
		printf("width: %d, height: %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

		MAX_NUM_OF_CHARS = (SCREEN_WIDTH/CHAR_WIDTH);
		MAX_NUM_OF_LINES = (SCREEN_HEIGHT/CHAR_HEIGHT);
	}

}

int main(int argc, char** argv)
{


	cursor_ctx_t cursor_ctx;

	data_structure_t ds;
	ds_init(&ds, 2000, 500);
	ds_insert_line(&ds, 0);

	cursor_ctx.row		 = 0;
	cursor_ctx.col		 = 0;
	cursor_ctx.font_size = 30;

	drawing_ctx_t drawing_ctx;
	drawing_ctx.row		  = 0;
	drawing_ctx.col		  = 0;
	drawing_ctx.row_size  = 20;
	drawing_ctx.col_size  = 20;


	Font courier;

	FILE* file;
	if(argc == 2)
	{
		const char* file_name = argv[1];
		file = fopen(file_name, "r");
	}
	else
	{
		file = fopen("/Users/jk/Programming/C/raylib/success.txt", "r");
	}


	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	ds_read_from_file(&ds, &file);
	fclose(file);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DO_NOT_RESIZE");
	courier = LoadFont("/Users/jk/programming/c/raylib/assets/courier_new.ttf");

	SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();

		//TODO: this is themes angle or sth
		ClearBackground(DARKPURPLE);
		{
			recalculateIfResized();
			process_key_press(&cursor_ctx, &ds);
			normalise_drawing(&cursor_ctx, &drawing_ctx);
			blink_cursor(&cursor_ctx, &drawing_ctx);
			render_data_structure(courier, &ds, drawing_ctx.col, drawing_ctx.row);
		}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
