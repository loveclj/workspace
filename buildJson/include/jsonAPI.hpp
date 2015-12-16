#ifndef __JSONAPI_HPP_
#define __JSONAPI_HPP_


#include<iostream>
#include<string>
#include<time.h>
#include<sstream>
#include<fstream>
#include<map>
#include<vector>
#include<sys/time.h>

#include<cstring>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include "stdarg.h"

#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

namespace JSONAPI
{

static const char *ENDFIELD = "\n";

/*
 * Function:        //GetFromValue
 * Description:     //get int, string or float from value
 * Input:           // Value or Document,  val
 * Output:          // val
 * Return:          //  if success,return 0, otherwise return -1
 * Others:          //  overload function
 */
int GetFromValue(Value &value,string &str);
int GetFromValue(Value &value,int &val);
int GetFromValue(Value &value,float &val);

/*
 * Function:        //doc2str
 * Description:     //convert json to string
 * Input:           // json
 * Output:          // val
 * Return:          //  string
 * Others:          //
 */
string  doc2str(Document &doc);


}




#endif // __JSONAPI_HPP_ #endif
