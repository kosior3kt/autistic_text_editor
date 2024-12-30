#include "data_structure.h"

void test1()
{

	printf("=====================================\n");
	printf("test1 - pushing and poping from dynamic array:\n");
	dynamic_array_t da;

	da_init(&da, 10);
	da_push_back(&da, 'a');
	da_push_back(&da, 'b');
	da_push_back(&da, 'c');
	da_pop(&da);

	printf("should be: a b: ");
	da_print(&da);

	char dupa = da_get_at(&da, 0);
	printf("this should be 'a' - %c\n", dupa);
	da_replace(&da, 0, 'g');
	dupa = da_get_at(&da, 0);
	printf("this should be 'g' - %c\n", dupa);
}

void test2()
{
	printf("=====================================\n");
	printf("test2 - inserting and deleting: \n");

	dynamic_array_t da;
	da_init(&da, 10);

	da_push_back(&da, 'a');
	da_push_back(&da, 'b');
	da_push_back(&da, 'c');
	da_push_back(&da, 'e');
	printf("should be 'a b c e' - ");
	da_print(&da);

	da_insert(&da, 3,'d');
	printf("should be 'a b c d e' - ");
	da_print(&da);

	da_remove(&da, 2);
	printf("should be 'a b d e' - ");
	da_print(&da);
}

void test3()
{
	printf("=====================================\n");
	printf("test3 - replacing characters:\n");

	dynamic_array_t da;
	da_init(&da, 10);

	da_push_back(&da, 'a');
	da_push_back(&da, 'b');
	da_push_back(&da, 'c');
	da_push_back(&da, 'e');
	printf("should be 'a b c e' - ");
	da_print(&da);

	da_replace(&da, 3, 'd');
	printf("should be 'a b c d' - ");
	da_print(&da);

	da_replace(&da, 0, 'g');
	printf("should be 'g b c d' - ");
	da_print(&da);
}

void test4()
{
	printf("=====================================\n");
	printf("test4 - creating, initializing and printing data structure:\n");
	data_structure_t ds;
	ds_init(&ds, 50, 50);

	ds_insert_line(&ds, 0);
	ds_insert_line(&ds, 1);

	ds_insert_char(&ds, 0, 0, 'a');
	ds_insert_char(&ds, 0, 1, 'b');
	ds_insert_char(&ds, 0, 2, 'c');

	ds_insert_char(&ds, 1, 0, 'd');
	ds_insert_char(&ds, 1, 1, 'e');
	ds_insert_char(&ds, 1, 2, 'f');
	ds_print(&ds);

	assert(ds.current_number_of_lines == 2);
}

void test4_2()
{	
	printf("=====================================\n");
	printf("test4_2 - deleting lines:\n");

	dynamic_array_t da;
	da_init(&da, 10);

	da_push_back(&da, 'a');
	da_push_back(&da, 'b');
	da_push_back(&da, 'c');
	da_push_back(&da, 'e');
	printf("should be 'a b c e' - ");
	da_print(&da);

	da_replace(&da, 3, 'd');
	printf("should be 'a b c d' - ");
	da_print(&da);

	da_replace(&da, 0, 'g');
	printf("should be 'g b c d' - ");
	da_print(&da);

}


void test5()
{
	printf("=====================================\n");
	printf("test5 - adding and deleting lines:\n");
	data_structure_t ds;
	ds_init(&ds, 50, 50);

	ds_insert_line(&ds, 0);
	ds_insert_char(&ds, 0, 0, 'a');
	ds_insert_char(&ds, 0, 1, 'b');
	ds_insert_char(&ds, 0, 2, 'c');

	ds_insert_line(&ds, 1);
	ds_insert_char(&ds, 1, 0, 'd');
	ds_insert_char(&ds, 1, 1, 'e');
	ds_insert_char(&ds, 1, 2, 'f');

	ds_insert_line(&ds, 2);
	ds_insert_char(&ds, 2, 0, 'g');
	ds_insert_char(&ds, 2, 1, 'h');
	ds_insert_char(&ds, 2, 2, 'i');
	printf("should be:\na b c\nd e f\ng h i\n");
	ds_print(&ds);
	printf("\n");

	ds_delete_line(&ds, 2);
	printf("should be:\na b c\nd e f\n");
	ds_print(&ds);
	printf("\n");

	ds_delete_line(&ds, 0);
	printf("should be:\nd e f\n");
	ds_print(&ds);
	printf("\n");

}


void test6()
{
	printf("=====================================\n");
	printf("test6 - splitting and merging lines:\n");
	data_structure_t ds;
	ds_init(&ds, 50, 50);

	ds_insert_line(&ds, 0);
	ds_insert_char(&ds, 0, 0, 'a');
	ds_insert_char(&ds, 0, 1, 'b');
	ds_insert_char(&ds, 0, 2, 'c');
	ds_insert_char(&ds, 0, 3, 'd');
	ds_insert_char(&ds, 0, 4, 'e');

	ds_insert_line(&ds, 1);
	ds_insert_char(&ds, 1, 0, 'f');
	ds_insert_char(&ds, 1, 1, 'g');
	ds_insert_char(&ds, 1, 2, 'h');
	ds_insert_char(&ds, 1, 3, 'i');
	ds_insert_char(&ds, 1, 4, 'j');

	printf("should be:\na b c d e\nf g h i j\n");
	ds_print(&ds);

	//fix this?
	ds_split_lines(&ds, 0, 3);
	printf("should be:\na b c\nd e\nf g h i j\n");
	ds_print(&ds);

	ds_merge_lines(&ds, 0);
	printf("should be:\na b c d e\nf g h i j\n");
	ds_print(&ds);

}

void test7()
{
	printf("=====================================\n");
	printf("test7 - removing and readding lines with the same index:\n");
	data_structure_t ds;
	ds_init(&ds, 50, 50);

	ds_insert_line(&ds, 0);
	da_push_back(&ds.lines[0], 'a');
	da_push_back(&ds.lines[0], 'b');
	da_push_back(&ds.lines[0], 'c');

	ds_insert_line(&ds, 1);
	da_push_back(&ds.lines[1], 'd');
	da_push_back(&ds.lines[1], 'e');
	da_push_back(&ds.lines[1], 'f');

	ds_insert_line(&ds, 2);
	da_push_back(&ds.lines[2], 'g');
	da_push_back(&ds.lines[2], 'h');
	da_push_back(&ds.lines[2], 'i');


	ds_delete_line(&ds, 2);
	ds_delete_line(&ds, 0);
	printf("current number of lines: %d\n", ds.current_number_of_lines);

	ds_print(&ds);

	ds_insert_line(&ds, ds.current_number_of_lines);
	printf("current number of lines: %d\n", ds.current_number_of_lines);
	ds_insert_line(&ds, ds.current_number_of_lines);
	printf("current number of lines: %d\n", ds.current_number_of_lines);
	ds_insert_line(&ds, ds.current_number_of_lines);
	printf("current number of lines: %d\n", ds.current_number_of_lines);

}




int main()
{
	printf("+++++++++++++++++++++++++++++++++++++\n");
	printf("dynamic array tests:\n");
	test1();
	test2();
	test3();

	printf("+++++++++++++++++++++++++++++++++++++\n");
	printf("data structure tests:\n");
	test4();
	test5();
	test6();
	test7();


	return 0;
}
