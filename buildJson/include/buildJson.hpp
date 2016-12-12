#ifndef _BUILDJSON_HPP_
#define _BUILDJSON_HPP_

#include "common.hpp"

template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, Document::AllocatorType & allocator);

template<typename Json>
void JsonAddObjByMap(Json &json, map<string, string> &fieldValue, Document::AllocatorType & allocator);

template<typename Json, typename T>
void JsonAddMember(Json *pjson, string field, T &value, Document::AllocatorType &allocator);

template<typename Json>
void JsonAddMember(Json *pjson, string field, string &value, Document::AllocatorType &allocator);

template<typename Json,typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, vector<string> fields,Document::AllocatorType & allocator);

template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, Document::AllocatorType & allocator, ...);

template<typename Json, typename T>
void 	JsonAddArray(Json *pjson, T element, Document::AllocatorType &allocator);

template<typename Json>
void 	JsonAddArray(Json *pjson, string element, Document::AllocatorType &allocator);

template<typename Json, typename T>
void JsonAddArrayByVector(Json &json, vector<T> &fieldValue, Document::AllocatorType & allocator, ...);



template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, Document::AllocatorType & allocator)
{
	typename map<string, T>::iterator  iter;
	for(iter = fieldValue.begin();iter != fieldValue.end(); iter++)
	{
		json.AddMember(iter->first.c_str(), iter->second, allocator);
	}
}

template<typename Json>
void JsonAddObjByMap(Json &json, map<string, string> &fieldValue, Document::AllocatorType & allocator)
{
	map<string, string>::iterator  iter;
	for(iter = fieldValue.begin();iter != fieldValue.end(); iter++)
	{
		json.AddMember(iter->first.c_str(), iter->second.c_str(), allocator);
	}
}
template<typename Json, typename T>
void JsonAddMember(Json *pjson, string field, T &value, Document::AllocatorType &allocator)
{
	pjson->AddMember(field.c_str(), value, allocator);
}

template<typename Json>
void JsonAddMember(Json *pjson, string field, string &value, Document::AllocatorType &allocator)
{
	pjson->AddMember(field.c_str(), value.c_str(), allocator);
}

template<typename Json,typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, vector<string> fields,Document::AllocatorType & allocator)
{
	;
	Value *v = &json;

	for(vector<string>::iterator iter = fields.begin(); iter != fields.end(); iter++)
	{
		const char * field = iter->c_str();
			if(!v->HasMember(field))
			{
				Value empty;
				empty.SetObject();
				v->AddMember(field,empty, allocator);
			}


			v =  &((*v)[field]);
	}

	for(typename map<string, T>::iterator  iter = fieldValue.begin(); iter != fieldValue.end(); iter++)
	{
		JsonAddMember(v,iter->first, iter->second, allocator);
	}
}


template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue, Document::AllocatorType & allocator, ...)
{

	va_list args;
	va_start(args, allocator);

	Value *v  = &json;
	const char *field;
	while (true)
	{
		field = va_arg(args,  const char *);
		if(strcmp(field, "\n") == 0)
		{
			break;
		}

		if (!v->HasMember(field))
		{
			Value empty;
			empty.SetObject();
			v->AddMember(field, empty, allocator);
		//	break;
		}

		v = &((*v)[field]);
	}
	va_end(args);

	for (typename map<string, T>::iterator iter = fieldValue.begin();
			iter != fieldValue.end(); iter++) {
		JsonAddMember(v, iter->first, iter->second, allocator);
	}

}

template<typename Json, typename T>
void 	JsonAddArray(Json *pjson, T element, Document::AllocatorType &allocator)
{
	pjson->PushBack(element, allocator);
}

template<typename Json>
void 	JsonAddArray(Json *pjson, string element, Document::AllocatorType &allocator)
{
	pjson->PushBack(element.c_str(), allocator);
}

template<typename Json, typename T>
void JsonAddArrayByVector(Json &json, vector<T> &fieldValue, Document::AllocatorType & allocator, ...)
{

	va_list args;
	va_start(args, allocator);

	Value *v  = &json;
	const char *field;
	while (true)
	{
		field = va_arg(args,  const char *);
		if(strcmp(field, "\n") == 0)
		{
			break;
		}

		if (!v->HasMember(field))
		{
			Value empty;
			empty.SetObject();
			v->AddMember(field, empty, allocator);
		//	break;
		}

		v = &((*v)[field]);
	}
	va_end(args);

	if(!v->IsArray())
	{
		v->SetArray();
	}

	for (typename vector<T>::iterator iter = fieldValue.begin();
			iter != fieldValue.end(); iter++) {

		JsonAddArray(v, *iter, allocator);
	}
}
#endif
