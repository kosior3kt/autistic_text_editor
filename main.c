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

//TODO: this should be macros that take into consideration size of font and other bs
#define DOC_HEIGHT  20
#define DOC_WIDTH	50

#define START_X		0
#define START_Y		0


// structures
typedef struct
{
	int row;
	int col;

	int font_size;
}cursor_ctx_t;




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

//I shouldnt do it this way really...
static void draw_cursor(const cursor_ctx_t* _ctx)
{
	DrawRectangle(col_to_coord(_ctx->row),
				  row_to_coord(_ctx->col),
				  CHAR_WIDTH, CHAR_HEIGHT - 3, 
				  GREEN);
}

static void render_char(Font _font, const char _glyph, int _row, int _col)
{
	if(_glyph == '\0') return;
	Vector2 temp_pos;
	temp_pos.y = row_to_coord(_row);
	temp_pos.x = col_to_coord(_col);

	//for now just ascii values as ints. lol
	DrawTextCodepoint(_font, (int)_glyph, temp_pos, 30, RED);
}

//TODO: is this passed as pointer or as value? insepct that in asm later
static void render_char_buffer(Font _font, 
							   const char _char_buf[DOC_HEIGHT * DOC_HEIGHT])
//TODO: is this fn still used? Is it good idea to use it?
{
	int i = 0;

	while(i < DOC_HEIGHT)
	{
		int j = 0;
		while(_char_buf[i * DOC_WIDTH + j] != '\0')
		{
			render_char(_font, _char_buf[i * DOC_WIDTH + j], i, j);
			++j;
		}
		++i;
	}
}

static void render_data_structure(Font _font, 
								  const data_structure_t* _data)
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
	while(key_pressed)
	{
		switch(key_pressed)
		{
			case KEY_ENTER:
			{
				++_ctx->col;
				_ctx->row = 0;

				//TODO: make it add after the current line instead
				ds_add_line(_ds);
				break;
			}
			case KEY_BACKSPACE:
			{
				if(_ctx->row > 0)
				{
					--_ctx->row;
					ds_remove_char(_ds, _ctx->col, _ctx->row);
				}
				else if(_ctx->col > 0)
				{
					ds_merge_lines(_ds, _ctx->col);

					--_ctx->col;

					//MAYBE: wrap it into function later?
					//  _ctx.row = ds_get_end_of_line(col_index);
					_ctx->row = _ds->lines[_ctx->col].current;
				}

				break;
			}
			case KEY_LEFT:
			{
				if(_ctx->row > 0)
					--_ctx->row;
					break;
			}
			case KEY_RIGHT:
			{
				//MAYBE: wrap it into function later?
				//  _ctx.row < ds_get_end_of_line(col_index);
				if(_ctx->row < _ds->lines[_ctx->col].current)
					++_ctx->row;
				break;
			}
			default:
			{
				key_pressed = GetCharPressed();

				ds_add_char(_ds, _ctx->col, 
							_ctx->row,
							(char)key_pressed);

				++_ctx->row;
				break;
			}
		}
		key_pressed = GetKeyPressed();
	}
/*
	for ( int key_pressed = GetKeyPressed(); 
		key_pressed != 0; 
		key_pressed = GetKeyPressed())
	{
		if(key_pressed == KEY_ENTER)
		{
			++_ctx->col;
			_ctx->row = 0;
			
			//TODO: make it add after the current line instead
			ds_add_line(_ds);
		}
		else if(key_pressed == KEY_BACKSPACE)
		{
			if(_ctx->row > 0)
			{
				--_ctx->row;
				ds_remove_char(_ds, _ctx->col, _ctx->row);
			}
			else if(_ctx->col > 0)
			{
				ds_merge_lines(_ds, _ctx->col);

				--_ctx->col;

				//MAYBE: wrap it into function later?
				//  _ctx.row = ds_get_end_of_line(col_index);
				_ctx->row = _ds->lines[_ctx->col].current;
			}
		}
		else if(key_pressed == KEY_LEFT)
		{
			if(_ctx->row > 0)
				--_ctx->row;
		}
		else if(key_pressed == KEY_RIGHT)
		{
			//MAYBE: wrap it into function later?
			//  _ctx.row < ds_get_end_of_line(col_index);
			if(_ctx->row < _ds->lines[_ctx->col].current)
				++_ctx->row;
		}
		else
		{
			key_pressed = GetCharPressed();

			ds_add_char(_ds, _ctx->col, 
						_ctx->row,
						(char)key_pressed);

			++_ctx->row;
		}
	}

*/
}

int main(void)
{
	cursor_ctx_t cursor_ctx;

	data_structure_t ds;
	ds_init(&ds, 20, 50);

	cursor_ctx.row		 = 0;
	cursor_ctx.col		 = 0;
	cursor_ctx.font_size = 30;

	Font courier;


    InitWindow(800, 450, "god help me");
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
