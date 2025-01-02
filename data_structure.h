#ifndef __DS__
#define __DS__

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

// ----------- Types
typedef struct
{
	int		max_size;
	int		current;
	char*	data;

} dynamic_array_t;

//UNDEFED AT THE END OF THE FILE
#define line_t dynamic_array_t

typedef struct
{

	int current_number_of_lines;
	int max_number_of_lines;

	line_t* lines;

} data_structure_t;


// ----------- File Interface

//technical
static void da_init(dynamic_array_t*, int);
static void da_print(const dynamic_array_t*);

static char da_get_at(const dynamic_array_t*, const uint8_t);
static void da_resize(dynamic_array_t*, const uint8_t);
static void da_replace(dynamic_array_t*, const uint8_t, const char);
static void da_push_back(dynamic_array_t*, const char);
static char da_pop(dynamic_array_t*);
// shifts everything
static void da_insert(dynamic_array_t*, const uint8_t, const char);
static void da_remove(dynamic_array_t*, const uint8_t);
//not implemented
static void da_add(dynamic_array_t*, const uint8_t, const char);

void ds_init(data_structure_t*, const int, const int);
void ds_print(const data_structure_t*);

void ds_replace_character(data_structure_t*, const uint8_t, const uint8_t, const char);
void ds_insert_line(data_structure_t*, const int);
void ds_delete_line(data_structure_t*, const uint8_t);
void ds_merge_lines(data_structure_t*, const uint8_t);
void ds_insert_char(data_structure_t*, const uint8_t, const uint8_t, const char);
void ds_push_back(data_structure_t*, const uint8_t, const char);
void ds_remove_char(data_structure_t*, const uint8_t, const uint8_t);
void ds_split_lines(data_structure_t*, const uint8_t, const uint8_t);

void ds_read_from_file(data_structure_t*, FILE**);
void ds_save_to_file(data_structure_t*, FILE**);

// ----------- start of dynamic array
//


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
		printf("array has size: %d, and max size: %d\n so implement resizing now ;3\n", 
		 (int)_array->current, (int)_array->max_size);
		//TODO: resize it later
		assert(0 == 1);
	}
	
	_array->data[_array->current] = _elem;
	
	++_array->current;
}

static char da_pop(dynamic_array_t* _array)
{
	if(_array->current <= 0)
	{
		//TODO: handle this some how
		assert(1 == 0);
	}

	if(_array->current <= (_array->max_size / 4))
	{
		//TODO: resize it again, if we care about memory
		//assert(1 == 0);
	}

	--_array->current;
	char ret_val = _array->data[_array->current];
	_array->data[_array->current] = '\0';
	return ret_val;
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

	assert(1 == 0);
	//TODO: implement resizing for this fucker. It' can't be this, since 
	// we need to realloc more memory and take care of dangling shit
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
	if(_pos > _array->current - 1)
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
	printf("===========================\n");
}

static void da_insert(dynamic_array_t* _array, 
					  const uint8_t _pos, 
					  const char _val)
{
	//TODO: check if this check is valid (indexes might be off or sth)
	if(_pos > _array->current)
	{
		// this shouldn't happend 
		assert(1 == 0);
	}

	//be careful this fucker is indexed from 0 when comes to data but from 
	//1 when comes to _array.current
	++_array->current;
	for(int i = _array->current - 1; i > _pos; --i)
	{
		_array->data[i] = _array->data[i - 1];
	}
	_array->data[_pos] = _val;
}

static void da_remove(dynamic_array_t* _array, const uint8_t _pos)
{
	//TODO: check if this check is valid (indexes might be off or sth)
	if(_pos > _array->current - 1)
	{
		// this shouldn't happend 
		assert(1 == 0);
	}

	for(int i = _pos; i < _array->current - 1; ++i)
	{
		_array->data[i] = _array->data[i + 1];
	}

	//remove last element
	_array->data[_array->current - 1] = '\0';
	--_array->current;
}

// ----------------- start of ds for editor


void ds_init(data_structure_t* _ds,
			 const int _num_of_lines, 
			 const int _num_of_chars)
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
		printf("line %d:\n", i);
		da_print(&_ds->lines[i]);
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

void ds_insert_line(data_structure_t* _ds, const int _line_index)
{
	if(_ds->current_number_of_lines + 1 >= _ds->max_number_of_lines)
	{
		// ass (hehe) soon as we hit this we need to make this fucker dynamicaly sized
		printf("\ncurrent numer of lines: %d, max declared lines: %d\n",
				_ds->current_number_of_lines,
				_ds->max_number_of_lines);
		assert(1 == 0);
	}

	if(_ds->current_number_of_lines < _line_index)
	{
		printf("current numer of lines: %d, requested line number: %d",
				_ds->current_number_of_lines,
				_line_index);
		assert(1 == 0);
	}

	// this is on purpose here, not earlier. thats cause lines 
	// are indexed from 1 in current_number_of_lines and from 0
	// in data structure. Be careful here (see i = current... - 1)
	++_ds->current_number_of_lines;

	for(int i = _ds->current_number_of_lines - 1; i > _line_index; --i)
	{
		_ds->lines[i] = _ds->lines[i - 1];
	}

	//TODO: fix this later
	da_init(&_ds->lines[_line_index], 5000);
}

void ds_delete_line(data_structure_t* _ds, const uint8_t _line_index)
{
	if(_line_index > _ds->current_number_of_lines)
	{
		//TODO: this seems like a problem
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
	
	// we start indexing actual lines from 0, right?
	if(_line_index < 0)
	{
		// shuold be unreachable
		assert(1 == 0);
	}

	if(_line_index >= _ds->current_number_of_lines)
	{
		//TODO: this seems like a problem 
		assert(1 == 0);
	}


	// take all characters from this line and append them in previous one by one
	for(int i = 0; i < _ds->lines[_line_index + 1].current; ++i)
	{
		// this would be cleaner done by poping first but it 
		// would have performance cost
		da_push_back(&_ds->lines[_line_index], 
			   _ds->lines[_line_index + 1].data[i]);
	}

	ds_delete_line(_ds, _line_index + 1);
}


void ds_insert_char(data_structure_t* _ds,
				const uint8_t _line_index,
				const uint8_t _char_index,
				const char _value)
{
	if(_line_index + 1 > _ds->current_number_of_lines)
	{
		printf("You request change in line that doesn't \
				exist yet in this data structure\n");

		printf("current amount of lines: %d, line with requested change: %d\n",
			_ds->lines[_line_index].current + 1,
			_line_index
		 );
		assert(1 == 0);
	}

	if(_ds->lines[_line_index].current < _char_index)
	{
		printf("line nr %d, has %d chars, but you requested change at index %d\n",
			_line_index,
			_ds->lines[_line_index].current,
			_char_index
		 );
		assert(1 == 0);
	}

	if(_ds->lines[_line_index].max_size <= _ds->lines[_line_index].current)
	{
		// nothing to be done here, we fucked up somewhere else
		printf("line nr %d, has %d chars capacity but already has %d\n",
			_line_index,
			_ds->lines[_line_index].max_size,
			_ds->lines[_line_index].current
		 );
		assert(1 == 0);
	}

	//da_insert updated amount of elelments it holds
	da_insert(&_ds->lines[_line_index], _char_index, _value);
}

void ds_push_back(data_structure_t* _ds, 
				  const uint8_t _line_index, 
				  const char _value)
{

	//TODO: check if errors make sense (they were copied from ds_insert_char)
	if(_line_index + 1 > _ds->current_number_of_lines)
	{
		printf("You request change in line that doesn't \
				exist yet in this data structure\n");

		printf("current amount of lines: %d, line with requested change: %d\n",
			_ds->lines[_line_index].current + 1,
			_line_index
		 );
		assert(1 == 0);
	}


	if(_ds->lines[_line_index].max_size <= _ds->lines[_line_index].current)
	{
		// nothing to be done here, we fucked up somewhere else
		printf("line nr %d, has %d chars capacity but already has %d\n",
			_line_index,
			_ds->lines[_line_index].max_size,
			_ds->lines[_line_index].current
		 );
		assert(1 == 0);
	}

	//da_insert updated amount of elelments it holds
	da_push_back(&_ds->lines[_line_index], _value);

}

void ds_remove_char(data_structure_t* _ds, 
				   const uint8_t _line_index, 
				   const uint8_t _char_index)
{
	if(_ds->current_number_of_lines < _line_index)
	{
		// this would mean we try to delete line that doesn't 'exist' yet
		assert(1 == 0);
	}
	else if(_ds->lines[_line_index].current <= _char_index + 1)
	{

		//TODO: this is more wrong but still not fully 
		// we shuold remove the whole line here or just early return?
		//assert(1 == 0);
		//
		printf("\n\nhot here\n\n");
		//--_ds->lines[_line_index].current;
		//why the fuck was this thing even here?
	}

	// make this fucker a null byte
	//_ds->lines[_line_index].data[_char_index] = '\0';
	da_remove(&_ds->lines[_line_index], _char_index);
}

void ds_split_lines(data_structure_t* _ds, 
					const uint8_t _line_index, 
					const uint8_t _char_index)
{

	//insert line below, equivalent of:
	//++_ds->current_number_of_lines;
	ds_insert_line(_ds, _line_index + 1);

	
	while(_ds->lines[_line_index].current > _char_index)
	{
		char temp_val = _ds->lines[_line_index].data[_char_index];
		ds_push_back(_ds, _line_index + 1, temp_val);
		da_remove(&_ds->lines[_line_index], _char_index);
	}
}

void ds_read_from_file(data_structure_t* _ds, FILE** _file)
{
	//TODO: change it later
	char buffer[256];
	memset(buffer, (char)0, sizeof(buffer));

	volatile int counter = 0;
	while(fgets(buffer, sizeof(buffer), *_file) != NULL)
	{
		printf("%d: %s", counter, buffer);
		for(int i = 0; i < sizeof(buffer); ++i)
		{
			if(buffer[i] == '\n') break;

			
			//this is tab
			if(buffer[i] == '\t')
			{
				for(int j = 0; j < 4; ++j)
				{
					da_push_back(
						&_ds->lines[_ds->current_number_of_lines - 1],
						' ');
				}
				continue;
			}
			da_push_back(
				&_ds->lines[_ds->current_number_of_lines - 1],
				buffer[i]);
		}
		ds_insert_line(_ds, _ds->current_number_of_lines);
		++counter;
	}
}

void ds_save_to_file(data_structure_t* _ds, FILE** _file)
{
	for(int i = 0; i < _ds->current_number_of_lines; ++i)
	{
		//TODO: change it later
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		for(int j = 0; j < _ds->lines[i].current; ++j)
		{
			buffer[j] += _ds->lines[i].data[j];
			if(buffer[j] == '\0') 
			{
				buffer[j] = '\n';
				buffer[j + 1] = '\0';
				break;
			}
		}
		const char* temp_thing = buffer;
		//fputs(temp_thing, *_file);
		//fwrite(buffer, sizeof(char), sizeof(buffer), *_file);
		fprintf(*_file, "%s\n", temp_thing);
	}

}


#undef line_t


#endif //__DS__
