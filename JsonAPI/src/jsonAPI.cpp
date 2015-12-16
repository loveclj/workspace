/*
 * jsonAPI.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: lizhifeng
 */

#include "jsonAPI.hpp"

namespace JSONAPI{

int GetFromValue(Value &value,string &str)
{
	if(!value.IsString())
	{
		return -1;
	}

	str = value.GetString();
	return 0;
}

int GetFromValue(Value &value,int &val)
{
	if(!value.IsInt())
	{
		return -1;
	}

	val = value.GetInt();
	return 0;
}

int GetFromValue(Value &value,float &val)
{
	if(!value.IsDouble())
	{
		return -1;
	}

	val = value.GetDouble();
	return 0;
}

string  doc2str(Document &doc)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    string str(buffer.GetString(), buffer.GetSize());
    return str;

}
}



