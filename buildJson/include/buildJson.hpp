#ifndef _BUILDJSON_HPP_
#define _BUILDJSON_HPP_

#include "common.hpp"

namespace JSONAPI {

const char *ENDFIELD = "\n";

/*
 * Function:        //JsonAddObjByMap
 * Description:     //add elements  of map into Value/Document, key as fields, value as value
 * Input:           // Value or Document,  map,  allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // type of second value in map could not be string or pointer,  JsonAddObjByMap<Json, String> forbedden
 */
template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		Document::AllocatorType & allocator);

/*
 * Function:        //JsonAddObjByMap
 * Description:     //add elements  of map into Value/Document, key as fields, value as value
 * Input:           // Value or Document,  map,  allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // type of second value in map is string, override JsonAddObjByMap<Json,T>
 */
template<typename Json>
void JsonAddObjByMap(Json &json, map<string, string> &fieldValue,
		Document::AllocatorType & allocator);

/*
 * Function:        //JsonAddMember
 * Description:     //add (key, value) into Value/Document
 * Input:           // Value or Document,  field, value,  allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // type of  value could not be string or pointer,JsonAddMember<Json,string> forbidden
 */
template<typename Json, typename T>
void JsonAddMember(Json *pjson, string field, T &value,
		Document::AllocatorType &allocator);

/*
 * Function:        //JsonAddMember
 * Description:     //add (key, value) into Value/Document
 * Input:           // Value or Document,  field, value,  allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // type of  value  is string, override JsonAddMember
 */

template<typename Json>
void JsonAddMember(Json *pjson, string field, string &value,
		Document::AllocatorType &allocator);

/*
 * Function:        //JsonAddObjByMap
 * Description:     //add map into Json[fields[0]][fields[1]][fields[2]]...., create if field not exsist
 * Input:           // Value or Document,  map, fields, allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // type of  value could  could not be string or pointer,JsonAddObjByMap<Json,string> forbidden
 */
template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		vector<string> fields, Document::AllocatorType & allocator);

/*
 * Function:        //JsonAddObjByMap
 * Description:     //add map into Json[fields1][fields2][fields3]...., create if field not exsist
 * Input:           // Value or Document,  map, allocator of Value or Document, field1, field2, ..., ENDFIELD
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // number of fields is unknown, the last parameter must be ENDFIELD
 */
template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		Document::AllocatorType & allocator, ...);

/*
 * Function:        //JsonAddArray
 * Description:     //add element into array
 * Input:           // Value or Document,  element, allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // JsonAddArray doesn't check pjson is array or not, if pjson is not array, fatal error occurs;
 *                     JsonAddArray<Json, string> forbidden
 */
template<typename Json, typename T>
void JsonAddArray(Json *pjson, T element, Document::AllocatorType &allocator);

/*
 * Function:        //JsonAddArray
 * Description:     //add a string  into array
 * Input:           // Value or Document,  element, allocator of Value or Document
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // JsonAddArray doesn't check pjson is array or not, if pjson is not array, fatal error occurs;
 */
template<typename Json>
void JsonAddArray(Json *pjson, string element,
		Document::AllocatorType &allocator);

/*
 * Function:        //JsonAddArrayByVector
 * Description:     //add vector  into Json[fields1][fields2][fields3]...., create if field not exsist
 * Input:           // Value or Document,  vector, allocator of Value or Document, field1, field2, ..., ENDFIELD
 * Output:          // Value or Document Modify
 * Return:          // null
 * Others:          // number of fields is unknown, the last parameter must be ENDFIELD
 */
template<typename Json, typename T>
void JsonAddArrayByVector(Json &json, vector<T> &fieldValue,
		Document::AllocatorType & allocator, ...);

template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		Document::AllocatorType & allocator) {
	typename map<string, T>::iterator iter;
	for (iter = fieldValue.begin(); iter != fieldValue.end(); iter++) {
		json.AddMember(iter->first.c_str(), iter->second, allocator);
	}
}

template<typename Json>
void JsonAddObjByMap(Json &json, map<string, string> &fieldValue,
		Document::AllocatorType & allocator) {
	map<string, string>::iterator iter;
	for (iter = fieldValue.begin(); iter != fieldValue.end(); iter++) {
		json.AddMember(iter->first.c_str(), iter->second.c_str(), allocator);
	}
}
template<typename Json, typename T>
void JsonAddMember(Json *pjson, string field, T &value,
		Document::AllocatorType &allocator) {
	pjson->AddMember(field.c_str(), value, allocator);
}

template<typename Json>
void JsonAddMember(Json *pjson, string field, string &value,
		Document::AllocatorType &allocator) {
	pjson->AddMember(field.c_str(), value.c_str(), allocator);
}

template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		vector<string> fields, Document::AllocatorType & allocator) {
	;
	Value *v = &json;

	for (vector<string>::iterator iter = fields.begin(); iter != fields.end();
			iter++) {
		const char * field = iter->c_str();
		if (!v->HasMember(field)) {
			Value empty;
			empty.SetObject();
			v->AddMember(field, empty, allocator);
		}

		v = &((*v)[field]);
	}

	for (typename map<string, T>::iterator iter = fieldValue.begin();
			iter != fieldValue.end(); iter++) {
		JsonAddMember(v, iter->first, iter->second, allocator);
	}
}

template<typename Json, typename T>
void JsonAddObjByMap(Json &json, map<string, T> &fieldValue,
		Document::AllocatorType & allocator, ...) {

	va_list args;
	va_start(args, allocator);

	Value *v = &json;
	const char *field;
	while (true) {
		field = va_arg(args, const char *);
		if (strcmp(field, "\n") == 0) {
			break;
		}

		if (!v->HasMember(field)) {
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
void JsonAddArray(Json *pjson, T element, Document::AllocatorType &allocator) {
	pjson->PushBack(element, allocator);
}

template<typename Json>
void JsonAddArray(Json *pjson, string element,
		Document::AllocatorType &allocator) {
	pjson->PushBack(element.c_str(), allocator);
}

template<typename Json, typename T>
void JsonAddArrayByVector(Json &json, vector<T> &fieldValue,
		Document::AllocatorType & allocator, ...) {

	va_list args;
	va_start(args, allocator);

	Value *v = &json;
	const char *field;
	while (true) {
		field = va_arg(args, const char *);
		if (strcmp(field, "\n") == 0) {
			break;
		}

		if (!v->HasMember(field)) {
			Value empty;
			empty.SetObject();
			v->AddMember(field, empty, allocator);
			//	break;
		}

		v = &((*v)[field]);
	}
	va_end(args);

	if (!v->IsArray()) {
		v->SetArray();
	}

	for (typename vector<T>::iterator iter = fieldValue.begin();
			iter != fieldValue.end(); iter++) {

		JsonAddArray(v, *iter, allocator);
	}
}

} // end JSONAPI
#endif
