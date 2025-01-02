// system stuff
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>

// raylib
#include "raylib.h"

// my stuff
#include "data_structure.h"

// defines
#define CHAR_HEIGHT	30
#define CHAR_WIDTH	16

#define START_X		0
#define START_Y		0
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define MAX_NUM_OF_LINES  20
#define MAX_NUM_OF_CHARAS 50
//TODO: clean these macros later on


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

static void draw_cursor(const cursor_ctx_t* _ctx)
{
	DrawRectangle(col_to_coord(_ctx->row),
				  row_to_coord(_ctx->col),
				  CHAR_WIDTH, CHAR_HEIGHT - 3, 
				  GREEN);

	printf("row: %d, col: %d\n", _ctx->row, _ctx->col);
}

static void render_char(Font _font, const char _glyph, int _row, int _col)
{
	if(_glyph == '\0') return;
	if(_glyph == (char)'\t');// no fucking clue what I was meant to do here
	Vector2 temp_pos;
	temp_pos.y = row_to_coord(_row);
	temp_pos.x = col_to_coord(_col);

	//for now just ascii values as ints. lol
	DrawTextCodepoint(_font, (int)_glyph, temp_pos, 30, RED);
}


static void render_data_structure(Font _font, const data_structure_t* _data)
{
	for(int i = 0; i < _data->current_number_of_lines; ++i)
	{
		for(int j = 0; j < _data->lines[i].current; ++j)
		{
			render_char(_font, _data->lines[i].data[j], i, j);
		}
	}
}

static void blink_cursor(const cursor_ctx_t* _ctx)
{
	// TODO: make it a fucntion that switches draw and not draw for 500ms at time
	// then draw every tiem cursor moves
	const double time_d = GetTime() * 1000;
	const int time = (int)time_d;

	if(time % 1000 > 500)
	{
		draw_cursor(_ctx);
	}
}

static void process_key_press(cursor_ctx_t* _ctx, data_structure_t* _ds)
{

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

			break;
		}
		case KEY_LEFT: //------------------------------------------------------
		{
			if(_ctx->row > 0)
			{
				--_ctx->row;
			}
			break;
		}
		case KEY_RIGHT: //-----------------------------------------------------
		{
			if(_ctx->row < _ds->lines[_ctx->col].current)
			{
				++_ctx->row;
			}
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
			break;
		}
		default: //------------------------------------------------------------
		{
			// this is how we get the magic keys from this mess
			// useful for shortcuts I guess
			int fancy_key_pressed = key_pressed;
			key_pressed = GetCharPressed();

			printf("fancy inserted: %d\n", fancy_key_pressed);

			// key_pressed = 0 means shift
			if(key_pressed == 0) break;
			printf("regular inserted: %d\n", key_pressed);
			printf("=============\n");

			ds_insert_char(_ds, _ctx->col, _ctx->row, (char)key_pressed);

			++_ctx->row;
			break;
		}
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

	ds_read_from_file(&ds, &file);
	fclose(file);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "god help me");
	courier = LoadFont("/Users/jk/programming/c/raylib/assets/courier_new.ttf");

	SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();

		//TODO: this is themes angle or sth
		ClearBackground(RAYWHITE);

		{
			process_key_press(&cursor_ctx, &ds);
			blink_cursor(&cursor_ctx);
			render_data_structure(courier, &ds);
		}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
