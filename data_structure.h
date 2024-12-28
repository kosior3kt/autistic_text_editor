#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>


// ----------- start of dynamic array

typedef struct
{
	int		max_size;
	int		current;
	char*	data;

} dynamic_array_t;

static void da_init(dynamic_array_t* _array, int _size)
{
	_array->current	 = 0;
	_array->max_size = _size;
	_array->data = (char*)malloc(sizeof(char) * _size);
	memset(_array->data, '\0', _size);
}

static void da_print(const dynamic_array_t* _array)
{
	for(int i = 0; i < _array->current; ++i)
	{
		printf("%c ", (char)_array->data[i]);
	}
	printf("\n");
}

static void da_push_back(dynamic_array_t* _array, const char _elem)
{
	if(_array->current >= _array->max_size)
	{
		//TODO: resize it later
		assert(0 == 1);
	}
	
	_array->data[_array->current] = _elem;
	
	++_array->current;
}

static void da_pop(dynamic_array_t* _array)
{
	if(_array->current <= 0)
	{
		//TODO: handle this some how
		assert(1 == 0);
	}

	if(_array->current <= (_array->max_size / 4))
	{
		//TODO: resize it again, if we care about memory
		assert(1 == 0);
	}

	--_array->current;
	_array->data[_array->current] = '\0';
}

static char da_get_at(const dynamic_array_t* _array, const uint8_t _index)
{
	//TODO: is uint8_t enough?
	if(_index >= _array->current)
	{
		//TODO: handle this later
		assert(1 == 0);
	}
	return _array->data[_index];
}

static void da_resize(dynamic_array_t* _array, const uint8_t _new_size)
{
	if(_new_size <= _array->current)
	{
		//TODO: handle this later
		assert(1 == 0);
	}
	_array->max_size = _new_size;
}

static void da_replace(dynamic_array_t* _array,
					   const uint8_t _pos,
					   const char _val)
{
	if(_pos > _array->current)
	{
		//TODO: handle this later
		assert(1 == 0);
	}

	_array->data[_pos] = _val;
}

static void da_add(dynamic_array_t* _array,
				   const uint8_t _pos,
				   const char _val)
{
	assert(1 == 0);
	// This thing shuoldn't even be possible taking into consideration
	// what this application is gonna do, but imma leave scuffolding 
	// for it
	if(_pos > _array->current)
	{
		//TODO: handle this later
		assert(1 == 0);
	}

	_array->data[_pos] = _val;
}

// ----------------- start of ds for editor

#define line_t dynamic_array_t
typedef struct
{

	int current_number_of_lines;
	int max_number_of_lines;

	line_t* lines;

} data_structure_t;

void ds_init(data_structure_t* _ds,
			 const uint8_t _num_of_lines, 
			 const uint8_t _num_of_chars)
{
	//You can default this to 50 and 50 or sth
	_ds->max_number_of_lines	 = _num_of_lines;
	_ds->current_number_of_lines = 0;
	_ds->lines = (line_t*)malloc(sizeof(line_t) * _num_of_lines);

	for(int i = 0; i < _num_of_lines; ++i)
	{
		da_init(&_ds->lines[i], _num_of_chars);
	}
}

void ds_print(const data_structure_t* _ds)
{
	for(int i = 0; i < _ds->current_number_of_lines; ++i)
	{
		da_print(&_ds->lines[i]);
		printf("next:\n\n");
	}
}

void ds_replace_character(data_structure_t* _ds,
					  const uint8_t _line_index,
					  const uint8_t _char_index,
					  const char _value)
{
	if(_ds->current_number_of_lines <= _line_index)
	{
		//TODO: handle this later. This should be allowed (technically)
		assert(1 == 0);
	}

	if(_ds->lines[_line_index].current <= _char_index)
	{
		//TODO: handle this later. This should be allowed (technically)
		assert(1 == 0);
	}

	da_replace(&_ds->lines[_line_index], _char_index, _value);
}

void ds_add_line(data_structure_t* _ds)
{
	++_ds->current_number_of_lines;
}

void ds_delete_line(data_structure_t* _ds, const uint8_t _line_index)
{
	if(_line_index >= _ds->current_number_of_lines)
	{
		//TODO: this seems like a problem however
		assert(1 == 0);
	}
	
	for(int i = _line_index; i < _ds->current_number_of_lines - 1; ++i)
	{
		_ds->lines[i] = _ds->lines[i + 1];
	}
	da_init(&_ds->lines[_ds->current_number_of_lines], 
		 _ds->lines[_ds->current_number_of_lines].max_size);

	--_ds->current_number_of_lines;
}

void ds_merge_lines(data_structure_t* _ds, const uint8_t _line_index)
{
	
	// this is used when we delete when cursor 
	// is in first position of the line. we start counting lines from 1 tho
	if(_line_index <= 0)
	{
		// shuold be unreachable
		assert(1 == 0);
	}

	if(_line_index >= _ds->current_number_of_lines)
	{
		//TODO: this seems like a problem however
		assert(1 == 0);
	}

	
	// take all characters from this line and append them in previous one by one
	for(int i = 0; i < _ds->lines[_line_index].current; ++i)
	{
		// this would be cleaner done by poping first but it 
		// would have performance cost
		da_push_back(&_ds->lines[_line_index - 1], 
			   _ds->lines[_line_index].data[i]);
	}

	ds_delete_line(_ds, _line_index);
}

void ds_split_lines(data_structure_t* _ds, 
					const uint8_t _line_index, 
					const uint8_t _char_index)
{

}

void ds_add_char(data_structure_t* _ds,
				const uint8_t _line_index,
				const uint8_t _char_index,
				const char _value)
{
	if(_ds->current_number_of_lines <= _line_index)
	{
		//TODO: fix this later (if needs)
		++_ds->current_number_of_lines;
	}

	if(_ds->lines[_line_index].current <= _char_index)
	{
		//TODO: fix this later (if needs)
		if(_ds->lines[_line_index].current + 1 < _char_index)
		{
			//TODO: idk. Thought?
			assert(1 == 0);
		}
		da_push_back(&_ds->lines[_line_index], _value);
	}

	da_replace(&_ds->lines[_line_index], _char_index, _value);
}

void ds_remove_char(data_structure_t* _ds, 
				   const uint8_t _line_index, 
				   const uint8_t _char_index)
{
	if(_ds->current_number_of_lines <= _line_index)
	{
		//TODO: this isn't necesaitly wrong but it's not correct either
		assert(1 == 0);
	}
	else if(_ds->lines[_line_index].current <= _char_index + 1)
	{
		//TODO: this is more wrong but still not fully 
		// we shuold remove the whole line here or just early return?
		//assert(1 == 0);
		--_ds->lines[_line_index].current;
	}

	// make this fucker a null byte
	_ds->lines[_line_index].data[_char_index] = '\0';
}

#undef line_t


