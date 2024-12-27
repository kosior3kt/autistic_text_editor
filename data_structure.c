#include "data_structure.h"

int main()
{

	{
		printf("test1:\n");
		dynamic_array_t da;
		da_init(&da, 10);
		da_push_back(&da, 'a');
		da_push_back(&da, 'b');
		da_push_back(&da, 'c');
		da_pop(&da);
		da_print(&da);

		char dupa = da_get_at(&da, 0);
		printf("this shuold be 'a': %c\n", dupa);
		da_replace(&da, 0, 'g');
		dupa = da_get_at(&da, 0);
		printf("this shuold be 'g': %c\n", dupa);
	}

	{
		printf("test2: \n");
		data_structure_t ds;
		ds_init(&ds, 50, 50);
		ds_add_char(&ds, 0, 0, 'a');
		ds_add_char(&ds, 0, 1, 'b');
		ds_add_char(&ds, 0, 2, 'c');

		ds_add_char(&ds, 1, 0, 'd');
		ds_add_char(&ds, 1, 1, 'e');
		ds_add_char(&ds, 1, 2, 'f');
		ds_print(&ds);
	}


	return 0;
}
