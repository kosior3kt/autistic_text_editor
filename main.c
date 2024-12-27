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




int main(void)
{

	cursor_ctx_t cursor_ctx;
	char buffer[DOC_HEIGHT][DOC_WIDTH];

	data_structure_t ds;
	ds_init(&ds, 20, 50);

	cursor_ctx.row		 = 0;
	cursor_ctx.col		 = 0;
	cursor_ctx.font_size = 30;

	Font courier;

	int row_index[DOC_HEIGHT];
	int col_index = 0;

	memset(row_index, 0, DOC_HEIGHT);
	memset(buffer, '\0', DOC_HEIGHT * DOC_WIDTH);


    InitWindow(800, 450, "god help me");

	courier = LoadFont("/Users/jk/programming/c/raylib/assets/courier_new.ttf");


    while(!WindowShouldClose())
    {
        BeginDrawing();
		{

			ClearBackground(RAYWHITE);

			for(int key_pressed = GetKeyPressed(); key_pressed != 0; key_pressed = GetKeyPressed())
			{
				if(key_pressed == KEY_ENTER)
				{
					++col_index;
					row_index[col_index] = 0;
					
					//ds
					ds_add_line(&ds);
				}
				else if(key_pressed == KEY_BACKSPACE)
				{
					if(row_index[col_index] > 0)
					{
						--row_index[col_index];
					}
					else if(col_index > 0)
					{
						--col_index;
					}

					buffer[col_index][row_index[col_index]] = '\0';

					// is it correct to allow it run with (0, 0)?
					//ds
					ds_remove_char(&ds, col_index, row_index[col_index]);

				}
				else if(key_pressed == KEY_LEFT)
				{
					if(row_index[col_index] != 0)
						--row_index[col_index];
				}
				else if(key_pressed == KEY_RIGHT)
				{
					if(row_index[col_index] != 0)
						++row_index[col_index];
				}
				else
				{
					buffer[col_index][row_index[col_index]] 
						= (char)key_pressed;

					//ds
					ds_add_char(&ds, col_index, 
								row_index[col_index],
								(char)key_pressed);

					++row_index[col_index];
				}
			}

			cursor_ctx.row = row_index[col_index];
			cursor_ctx.col = col_index;
			
		}

		blink_cursor(&cursor_ctx);
		/*

		char buffer2[20 * 50];

		for(int i = 3; i < 255; ++i)
		{
				buffer2[i] = i - 3;
		}
		render_char_buffer(courier, buffer2);
		*/

		render_data_structure(courier, &ds);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
