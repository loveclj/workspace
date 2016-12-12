#include "amz_date.hpp"


string GetAmzDate() // get time in ISO8061
{
    time_t      cur_time  = time(NULL);
    struct tm  *cur_tm    = gmtime(&cur_time);

    stringstream date_sstream;

    AddDateElement(date_sstream, cur_tm->tm_year + 1900);
    AddDateElement(date_sstream, cur_tm->tm_mon + 1);
    AddDateElement(date_sstream, cur_tm->tm_mday);
    date_sstream << "T";

    AddDateElement(date_sstream, cur_tm->tm_hour);
    AddDateElement(date_sstream, cur_tm->tm_min);
    AddDateElement(date_sstream, cur_tm->tm_sec);
    date_sstream << "Z";

    return date_sstream.str();
}

string GetDateStamp(const string & amz_date)
{
    string stamp(amz_date.begin(), amz_date.begin() + 8);
    return stamp;
}

inline void AddDateElement(stringstream &date_sstream, int element)
{
    if(element < 10 )  date_sstream << 0;

    date_sstream << element;
}
