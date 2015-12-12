/*************************************************************************
	> File Name: test_docoment.cpp
	> Author: Arcs
	> Mail: lizhifeng2009@126.com
	> Created Time: Fri 04 Dec 2015 02:30:17 PM CST
 ************************************************************************/

#include<iostream>

#include"./rapidjson/document.h"

#include"./rapidjson/allocators.h"

#include"./rapidjson/stringbuffer.h"

#include"./rapidjson/writer.h"

#include"./rapidjson/rapidjson.h"

#include "./rapidjson/reader.h"

#include"./rapidjson/prettywriter.h"

using namespace std;

using namespace rapidjson;

#if 0
int main(int argc, char **argv)
{
    Document d;
    d.SetObject();
    Value vElem;
    vElem.SetArray();

    Document::AllocatorType & allocator = d.GetAllocator();
    for(int i = 0; i < 10; ++i)
    {
        Value tmp;///(i);
        tmp.SetInt(i);
      //  vElem.PushBack<int>(i, d.GetAllocator);
        vElem.PushBack(tmp, allocator);
    }
    d.AddMember("Data", vElem, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    string json(buffer.GetString(), buffer.GetSize());
    cout << json << endl;
 //   Document::AllocatorType& a = d.GetAllocator();
 //  cout <<  d["test"].IsNull() << endl;// = 1;

 //   Value v1("foo");
  //  Value v2(v1, a);

    



}
#endif
