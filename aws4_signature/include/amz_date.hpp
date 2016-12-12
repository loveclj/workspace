#ifndef __AMZ_DATE__
#define __AMZ_DATE__

#include<sstream>
#include<sys/time.h>
#include<time.h>
#include<string>

using namespace std;

string GetAmzDate(); // get time in ISO8061

string GetDateStamp(const string & amz_date);

inline void AddDateElement(stringstream &date_sstream, int element);


#endif //end __AMZ_DATE__
