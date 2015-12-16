/*
 * sparseJson.hpp
 *
 *  Created on: Dec 16, 2015
 *      Author: lizhifeng
 */


#ifndef SPARSEJSON_HPP_
#define SPARSEJSON_HPP_

#include "jsonAPI.hpp"

using namespace std;
using namespace rapidjson;

namespace JSONAPI{



/*
 * Function:        //GetFromJson
 * Description:     //get json[field1][field2][field3] value
 * Input:           // json, val, field1, field2,..., ENDFIELD
 * Output:          // val
 * Return:          // if success,return 0, otherwise return -1
 * Others:          // the last variable must be ENDFIELD
 */
template< typename Json, typename T>
int GetFieldFromJson(Json &json, T &val, ...);

/*
 * Function:        //GetJsonFromJson
 * Description:     //get json[field1][field2][field3] as json
 * Input:           // json, val, field1, field2,..., ENDFIELD
 * Output:          // val(json type)
 * Return:          // if success,return 0, otherwise return -1
 * Others:          // the last variable must be ENDFIELD
 */
template< typename Json>
int GetJsonFromJson(Json &json,Value * &val, ...);

/*
 * Function:        //GetArrayFromJson
 * Description:     //get json[field1][field2][field3]  as array
 * Input:           // json, val, field1, field2,..., ENDFIELD
 * Output:          // val(array type)
 * Return:          // if success,return 0, otherwise return -1
 * Others:          // the last variable must be ENDFIELD
 */
template< typename Json>
int GetArrayFromJson(Json &json,Value *&val, ...);

/*
 * Function:        //GetMapFromJson
 * Description:     //get json[field1][field2][field3] as map
 * Input:           // json, val, field1, field2,..., ENDFIELD
 * Output:          // kv map, store field as first value, value as second value
 * Return:          // if success,return 0, otherwise return -1
 * Others:          // the last variable must be ENDFIELD
 */
template< typename Json, typename T>
int GetMapFromJson(Json &json,map<string, T> &kv, ...);

/*
 * Function:        //GetMapFromJson
 * Description:     //get json[field1][field2][field3] as vector
 * Input:           // json, vec, field1, field2,..., ENDFIELD
 * Output:          // vector<T> vec, store value as elements of vec
 * Return:          // if success,return 0, otherwise return -1
 * Others:          // the last variable must be ENDFIELD
 */
template< typename Json, typename T>
int GetVectorFromJson(Json &json, vector<T> &vec, ...);




template< typename Json, typename T>
int GetFieldFromJson(Json &json, T &val, ...)
{
	va_list args;
		va_start(args, val);

		Value *v = &json;
		const char *field;
		while (true) {
			field = va_arg(args, const char *);
			if (strcmp(field, ENDFIELD) == 0) {
				break;
			}

			if (!v->HasMember(field)) {
				return -1;
			}

			v = &((*v)[field]);
		}
		va_end(args);

		int ret = GetFromValue(*v, val);
		return ret;
}

template< typename Json>
int GetJsonFromJson(Json &json,Value * &val, ...)
{
	va_list args;
		va_start(args, val);

		val  = &json;
		const char *field;
		while (true) {
			field = va_arg(args, const char *);
			if (strcmp(field, ENDFIELD) == 0) {
				break;
			}

			if (!val->HasMember(field)) {
				return -1;
			}

			val = &((*val)[field]);
		}
		va_end(args);

		if(val->IsObject())
		{
			return 0;
		}
		else
		{
			return -1;
		}
}

template< typename Json>
int GetArrayFromJson(Json &json,Value * &val, ...)
{
	va_list args;
		va_start(args, val);

		val  = &json;
		const char *field;
		while (true) {
			field = va_arg(args, const char *);
			if (strcmp(field, ENDFIELD) == 0) {
				break;
			}

			if (!val->HasMember(field)) {
				return -1;
			}

			val = &((*val)[field]);
		}
		va_end(args);

		if(val->IsArray())
		{
			return 0;
		}
		else
		{
			return -1;
		}
}

template< typename Json, typename T>
int GetMapFromJson(Json &json,map<string, T> &kv, ...)
{
	va_list args;
		va_start(args, kv);

		Value *val  = &json;
		const char *field;
		while (true) {
			field = va_arg(args, const char *);
			if (strcmp(field, ENDFIELD) == 0) {
				break;
			}

			if (!val->HasMember(field)) {
				return -1;
			}

			val = &((*val)[field]);
		}
		va_end(args);

		if(!val->IsObject())
		{
			return -1;
		}

		Document::MemberIterator iter;
		for(iter = val->MemberBegin(); iter != val->MemberEnd(); iter++)
		{
			Value &field = iter->name;
			Value &value = iter->value;

			string k;
			T      v;
			GetFromValue(field, k);
			GetFromValue(value, v);
			kv.insert(make_pair(k,v));
		}
		return 0;
}

template< typename Json, typename T>
int GetVectorFromJson(Json &json, vector<T> &vec, ...)
{
	va_list args;
		va_start(args, vec);

		Value *val  = &json;
		const char *field;
		while (true) {
			field = va_arg(args, const char *);
			if (strcmp(field, ENDFIELD) == 0) {
				break;
			}

			if (!val->HasMember(field)) {
				return -1;
			}

			val = &((*val)[field]);
		}
		va_end(args);

		if(!val->IsArray())
		{
			return -1;
		}

		rapidjson::SizeType i = 0;
		rapidjson::SizeType total_num = val->Size();
		vec.reserve(total_num);
		for(rapidjson::SizeType i = 0; i < total_num; i++)
		{
			Value &element = (*val)[i];
			string str;
			GetFromValue(element, str);
			vec.push_back(str);
		}

		return 0;
}

}// end JSONAPI
#endif /* ifndef SPARSEJSON_HPP_ */
