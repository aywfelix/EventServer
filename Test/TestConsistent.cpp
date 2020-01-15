#include "SFTest.h"
#include "MapEx.hpp"
#include <iostream>

class server_t
{
public:
	server_t(int id, std::string name):m_id(id),m_name(name){}
	int m_id;
	std::string m_name;
};
using data_ptr = std::shared_ptr<server_t>;

void SFTest::TestConsistent()
{

	//Map<int, server_t> mm;
	//server_t data1; data1.data = 1;
	//mm.insert(1, &data1);
	//server_t* data2 = mm.get_value(1);
	//std::cout << data2->data << std::endl;

	ConsistentHashEx<int, server_t> hashmap;

	server_t server1(1, "server1");
	server_t server2(2, "server2");
	server_t server3(3, "server3");
	server_t server4(4, "server4");
	server_t server5(5, "server5");
	server_t server6(6, "server6");
	server_t server7(7, "server7");
	server_t server8(8, "server8");
	server_t server9(9, "server9");

	hashmap.insert(1, server1);
	hashmap.insert(2, server2);
	hashmap.insert(3, server3);
	hashmap.insert(4, server4);
	hashmap.insert(5, server5);
	hashmap.insert(6, server6);
	hashmap.insert(7, server7);
	hashmap.insert(8, server8);
	hashmap.insert(9, server9);

	data_ptr ptr = nullptr;
	ptr = hashmap.get_server_by_key('aaa');
	if (ptr) std::cout << ptr->m_name<< std::endl;

	ptr = hashmap.get_server_consistent();
	if(ptr) std::cout << ptr->m_name << std::endl;

	ptr = hashmap.get_server_random();
	if (ptr) std::cout << ptr->m_name << std::endl;

	hashmap.remove(8);

	ptr = hashmap.get_server_by_key(8);
	if(ptr) std::cout << ptr->m_name << std::endl;
}