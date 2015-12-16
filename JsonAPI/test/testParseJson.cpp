/*
 * testParseJson.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: lizhifeng
 */


#include "test/testParseJson.hpp"
#include "buildJson.hpp"



void testGetFieldFromJson()
{
	Document doc;
	doc.SetObject();
	Document::AllocatorType &allocator = doc.GetAllocator();

	string field1("field1");
	string field2("field2");
	string field3("field3");
	string field4("field4");

	string first("key1");
	string second("value");

	JsonAddObjByKeyValue(doc, first, second, allocator, field1.c_str(), field2.c_str() , ENDFIELD);
	string str;
	GetFieldFromJson(doc, str, field1.c_str(), field2.c_str(), first.c_str(), ENDFIELD );

	cout << str << endl;
	cout << doc2str(doc) << endl;

}
void testGetJsonFromJson()
{
	Value *value;

	Document doc;
		doc.SetObject();
		Document::AllocatorType &allocator = doc.GetAllocator();

		string field1("field1");
		string field2("field2");
		string field3("field3");
		string field4("field4");

		string first("key1");
		string second("value");

		JsonAddObjByKeyValue(doc, first, second, allocator, field1.c_str(), field2.c_str() , ENDFIELD);
		GetJsonFromJson(doc, value, field1.c_str(), ENDFIELD);
		value->AddMember("ee","bb", allocator);
		cout << doc2str(doc) << endl;

}
void testGetXXXXFromJson()
{
	vector<int> v_int;
		int N = 5;
		for(int i = 0; i < N; ++i)
		{
			v_int.push_back(i);
		}

		vector<string> v_str;

		for(int i = 0; i < N; ++i)
		{
			stringstream ss;
			ss << i << "b";
			string str;
			ss >> str;
			v_str.push_back(str);
		}

		Document doc;
		doc.SetObject();
		Document::AllocatorType &allocator = doc.GetAllocator();


		/*
		 * test GetArrayFromJson
		 */
		JsonAddArrayByVector(doc,v_int, allocator,"field1","field2", ENDFIELD);
		JsonAddArrayByVector(doc,v_str, allocator,"fielda","fieldb", ENDFIELD);

		Value *array;
		GetArrayFromJson(doc, array, "field1", "field2", ENDFIELD );

		for(rapidjson::SizeType i = 0; i < array->Size(); ++i)
		{
			cout << (*array)[i].GetInt() << endl;
		}

		/*
		 * test GetVectorFromJson
		 */
		vector<string> v_str_parse;
		GetVectorFromJson(doc, v_str_parse, "fielda", "fieldb", ENDFIELD);

		for(vector<string>::iterator iter = v_str_parse.begin(); iter != v_str_parse.end(); ++iter)
		{
			cout << *iter << endl;
		}


		map<string, string> str_str;
		str_str["china"] = "Asian";
		str_str["American"] = "America";
		str_str["Russia"]   = "Europe";
		JsonAddObjByMap(doc,str_str, allocator,"test1", "test2", ENDFIELD);

		/*
		 * test GetMapFromJson
		 */

		map<string, string> str_str_parse;
		GetMapFromJson(doc, str_str_parse, "test1", "test2", ENDFIELD);

		for(map<string, string>::iterator iter = str_str_parse.begin(); iter != str_str_parse.end(); iter++)
		{
			cout << iter->first << " : " << iter->second << endl;
		}



}


