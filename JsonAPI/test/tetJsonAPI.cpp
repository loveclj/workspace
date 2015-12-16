/*
 * tetJsonAPI.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: lizhifeng
 */

#include "test/testJsonAPI.hpp"

void testGetFromValue()
{
	Document doc;
	doc.SetObject();

	Document::AllocatorType &allocator = doc.GetAllocator();

	doc.AddMember("int", 1, allocator);
	doc.AddMember("float", 2.3, allocator);
	doc.AddMember("string", "test", allocator);

	int i;
	float f;
	string s;


	GetFromValue(doc["int"], i);
	GetFromValue(doc["float"], f);
	GetFromValue(doc["string"], s);

	cout << "int   :" << i << endl;
	cout << "string:" << s << endl;
	cout << "float :" << f << endl;



}

