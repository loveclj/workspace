/*
 * testBuildJson.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: lizhifeng
 */


#include "test/testBuildJson.hpp"


void test_JsonAddMember() {
	Document doc;
	doc.SetObject();

	string field1("A");
	string field2("B");
	string field3("C");
	int value1(1);
	float value2(2.3);
	string value3("test");

	/*
	 * test JsonAddMember<Document,T>
	 */
	Document::AllocatorType &allocator = doc.GetAllocator();

	JsonAddMember(doc, field1, value1, allocator);
	JsonAddMember(doc, field2, value2, allocator);
	JsonAddMember(doc, field3, value3, allocator);



	/*
	 * test JsonAddMember<Value,T>
	 */
	string secondField("second");

	Value tmp;
	tmp.SetObject();
	JsonAddMember(doc, secondField, tmp, allocator);

	Value &second = doc[secondField.c_str()];

	JsonAddMember(second, field1, value1, allocator);
	JsonAddMember(second, field2, value2, allocator);
	JsonAddMember(second, field3, value3, allocator);

	string jsonStr = doc2str(doc);
	cout << jsonStr << endl;

}


void test_JsonAddObjByMap()
{
	map<string, int> str_int;
	str_int["a"] = 1;
	str_int["b"] = 2;
	str_int["c"] = 3;

	Document doc;
	doc.SetObject();
	Document::AllocatorType & allocator = doc.GetAllocator();

	/*
	 * test Document, int
	 */
	JsonAddObjByMap(doc,str_int, allocator,ENDFIELD);

	JsonAddObjByMap(doc,str_int, allocator,"test1", "test2", ENDFIELD);


	/*
	 * test  Value, int
	 */
	string secondField("second");

	Value tmp;
	tmp.SetObject();
	JsonAddMember(doc, secondField, tmp, allocator);

	Value &second = doc[secondField.c_str()];
	JsonAddObjByMap(second,str_int, allocator,ENDFIELD);

	JsonAddObjByMap(second,str_int, allocator,"test1", "test2", ENDFIELD);



	map<string, string> str_str;
	str_str["china"] = "Asian";
	str_str["American"] = "America";
	str_str["Russia"]   = "Europe";

	/*
	 * test Doc,string
	 */
	JsonAddObjByMap(second,str_str, allocator,ENDFIELD);
	JsonAddObjByMap(doc,str_str, allocator,"test1", "test2", ENDFIELD);

	/*
	 * test value, string
	 */
	JsonAddObjByMap(second,str_str, allocator,ENDFIELD);
	JsonAddObjByMap(second,str_str, allocator,"test2", "test2", ENDFIELD);


	cout << doc2str(doc) << endl;

}

void  testJsonAddArray()
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator();
	Value array;
	array.SetArray();


	JsonAddArray(array, 1,allocator);
	JsonAddArray(array, "2",allocator);
	JsonAddArray(array, "test",allocator);

	string field1("array1");
	JsonAddMember(doc, field1, array, allocator);


	Value array2;
	array2.SetArray();
	string field2("array2");
	JsonAddMember(doc, field2, array2, allocator);

	Value &cp = doc["array2"];
	JsonAddArray(cp, 1,allocator);
	JsonAddArray(cp, "2",allocator);
	JsonAddArray(cp, "test",allocator);


	cout << doc2str(doc) << endl;

}



void testJsonAddArrayByVector()
{
	vector<int> v_int;
	int N = 5;
	for(int i = 0; i < N; ++i)
	{
		v_int.push_back(i);
	}

	vector<string> v_str;

	for(int i = -0; i < N; ++i)
	{
		stringstream ss;
		ss << i;
		string str;
		ss >> str;
		v_str.push_back(str);
	}

	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator();
	Value array;
	array.SetArray();

	/*
	 * test value
	 */

	JsonAddArrayByVector(array,v_int, allocator,ENDFIELD);
	JsonAddArrayByVector(array,v_str, allocator, ENDFIELD);

	doc.AddMember("array", array, allocator);

	/*
	 * test Document
	 */
	JsonAddArrayByVector(doc,v_int, allocator,"field1","field2", ENDFIELD);
	JsonAddArrayByVector(doc,v_str, allocator,"fielda","fieldb", ENDFIELD);

	cout << doc2str(doc) << endl;
}
