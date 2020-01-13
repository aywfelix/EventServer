#include "SF_Test.h"
#include "MapEx.hpp"
#include <iostream>

class data_t
{
public:
	int data;
};
using data_ptr = std::shared_ptr<data_t>;

void SF_Test::TestConsistent()
{

	//Map<int, data_t> mm;
	//data_t data1; data1.data = 1;
	//mm.insert(1, &data1);
	//data_t* data2 = mm.get_value(1);
	//std::cout << data2->data << std::endl;

	ConsistentHashEx<int, data_t> hashmap;

	data_t data1; data1.data = 1;
	data_t data2; data2.data = 2;
	data_t data3; data3.data = 3;
	data_t data4; data4.data = 4;
	data_t data5; data5.data = 5;
	data_t data6; data6.data = 6;
	data_t data7; data7.data = 7;
	data_t data8; data8.data = 8;
	data_t data9; data9.data = 9;
	hashmap.insert(1, data1);
	hashmap.insert(2, data2);
	hashmap.insert(3, data3);
	hashmap.insert(4, data4);
	hashmap.insert(5, data5);
	hashmap.insert(6, data6);
	hashmap.insert(7, data7);
	hashmap.insert(8, data8);
	hashmap.insert(9, data9);

	data_ptr ptr1 = hashmap.get_server_by_key(2);
	if (ptr1) std::cout << ptr1->data << std::endl;

	data_ptr ptr2 = hashmap.get_server_consistent();
	std::cout << ptr2->data << std::endl;

	data_ptr ptr3 = hashmap.get_server_random();
	std::cout << ptr3->data << std::endl;

	hashmap.remove(8);

	data_ptr ptr4 = hashmap.get_server_by_key(8);
	if(ptr4) std::cout << ptr4->data << std::endl;
}