/*************************************************************************
	> File Name: poco_http.cpp
	> Author: Arcs
	> Mail: lizhifeng2009@126.com
	> Created Time: Wed 02 Dec 2015 10:13:31 AM CST
 ************************************************************************/
#include<iostream>
#include<string>
#include<time.h>
#include<sstream>
#include<fstream>
#include<map>
#include<sys/time.h>

#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"
#include "Poco/HMACEngine.h"

#include "./test_document.cpp"
#include "./awsv4.hpp"

using namespace std;
using namespace Poco::Net;
using namespace Poco;

string  doc2str(Document &doc)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    string str(buffer.GetString(), buffer.GetSize());
    return str;

}

void addDateElement(stringstream &date_sstream, int element)
{
    if(element < 10 )  date_sstream << 0;

    date_sstream << element;
}

string getAmzDate() // get time in ISO8061
{
    time_t      cur_time  = time(NULL);
    struct tm  *cur_tm    = gmtime(&cur_time);

    stringstream date_sstream;

    addDateElement(date_sstream, cur_tm->tm_year + 1900);
    addDateElement(date_sstream, cur_tm->tm_mon + 1);
    addDateElement(date_sstream, cur_tm->tm_mday);
    date_sstream << "T";

    addDateElement(date_sstream, cur_tm->tm_hour);
    addDateElement(date_sstream, cur_tm->tm_min);
    addDateElement(date_sstream, cur_tm->tm_sec);
    date_sstream << "Z";

    return date_sstream.str();
}

string getDateStamp(const string & amz_date)
{
    string stamp(amz_date.begin(), amz_date.begin() + 8);
    return stamp;
}

void setRequestHeader(HTTPRequest &request,string& http_body, string access_key, string secret)
{
			string method("POST");
			string service("dynamodb");
			//string host("dynamodb.cn-north-1.amazonaws.com.cn");
			string host("192.168.6.170");
			//string host("dynamodb.cn-north-1.amazonaws.cn");
			string uri("/");
			string region("cn-north-1");
			string content_type("application/x-amz-json-1.0");
			string amz_target("DynamoDB_20120810.Scan");
			string query_string("");
			string algorithm("AWS4-HMAC-SHA256");

			string amz_date = getAmzDate();
			string amz_stamp = getDateStamp(amz_date);
			//string amz_date = "20151212Z0922T";
			//string amz_stamp = "20151212";//getDateStamp(amz_date);

			string cannocial_header("");
			cannocial_header += "content-type:" + content_type + "\n" + \
					             "host:" + host + "\n" + \
					             "x-amz-date:" + amz_date + "\n" + \
					             "x-amz-target:" + amz_target + "\n";

			string signed_headers("content-type;host;x-amz-date;x-amz-target");

			string sha256("SHA256");
			//Poco::Crypto::DigestEngine engine(sha256);

			//engine.update(http_body);

			//Poco::DigestEngine::Digest payload = engine.digest();
			//string payload_hash((Poco::Crypto::DigestEngine::digestToHex(payload)));

			string payload_hash = AWSV4::sha256_base16(http_body);

			string canonical_request  = method + "\n" + uri + "\n"  + \
					                    query_string  +"\n" + cannocial_header + "\n" + \
					                    signed_headers + "\n" + payload_hash;

			//engine.update(canonical_request);
			//Poco::DigestEngine::Digest canonical_request_digest = engine.digest();
			//string canonical_request_hash(Poco::Crypto::DigestEngine::digestToHex((canonical_request_digest)));
			string canonical_request_hash= AWSV4::sha256_base16(canonical_request);
			string credential_scope = amz_stamp + "/" + region + "/" + service + "/" + "aws4_request";
			string string_to_sign   = algorithm + "\n" + amz_date + "\n" + credential_scope + "\n" + canonical_request_hash;


			string signature = AWSV4::calculate_signature(amz_stamp,
					   	   	   	   	   	   	   	   	   	    secret,
			                                                region,
			                                                service,
			                                                string_to_sign);


			cout << signature << endl;


			cout << amz_stamp << endl;
			cout << region << endl;
			cout << service << endl;
			cout << secret << endl;
			cout << string_to_sign << endl;

			string authorization_header = algorithm + " " + "Credential=" + access_key + "/" + \
					                      credential_scope + "," + "SignedHeaders=" + signed_headers + ", " + \
					                      "Signature=" + signature;

			cout << authorization_header  << endl;

 //   HTTPRequest request(HTTPRequest::HTTP_POST, "/", HTTPRequest::HTTP_1_1);
    request.setVersion(HTTPRequest::HTTP_1_1);
    request.setURI(uri.c_str());
    request.setMethod(HTTPRequest::HTTP_POST);
 //   request.set("Method", HTTPRequest::HTTP_POST);
    request.set("X-Amz-Date", amz_date.c_str());
    request.set("X-Amz-Target", amz_target.c_str());
    request.set("Authorization", authorization_header.c_str());
}

void setSession(HTTPClientSession &session, string &host, Poco::UInt16 port )
{
    session.setHost(host);
    session.setPort(port);
}

void ValueAddMapArray(Value &target, Value &element, Document::AllocatorType & allocator)
{
 //   target.push_back(, element, allocator);
}


int main(int argc, char **argv)
{
	 Document doc;
	 doc.SetObject();
	 Document::AllocatorType & allocator = doc.GetAllocator();

	 doc.AddMember("TableName", "patent", allocator);
	 doc.AddMember("ProjectionExpression", "patent_id", allocator);


	 //   doc.AddMember("")
	string body = doc2str(doc);


	string access_key;
	string secret;
	fstream secretfile("./conf/keys.text");
	secretfile >> access_key;
	secretfile >> secret;
	//secret = "oHd73jvubb/RhgzeHsuAKCAELBGuI4qAhaeE3hvT";
	if(access_key.empty() || secret.empty() )
	{
		cout << "aws: access id or secret key empty " << endl;
		return -1;
	}


    //cout << body << endl;

    //doc.AddMember("Limit", 1000, allocator);


    string dbHost("192.168.6.170");
    Poco::UInt16 dbPort(8000);

    HTTPClientSession session;
    setSession(session, dbHost, dbPort);

    HTTPRequest request;
    setRequestHeader(request,body, access_key, secret);
    request.setContentLength(body.length());

    ostream & os =  session.sendRequest(request);
    os << body;

    HTTPResponse res;
    istream &is = session.receiveResponse(res);

    int statusCode = (int)res.getStatus();
    string status = res.getReason();
    cout << status << endl;

    cout << statusCode << endl;

    string uuid_str;
    is >> uuid_str;
 //   cout << uuid_str << endl;

    //parse return
    Document uuid_doc;
    uuid_doc.Parse<0>(uuid_str.c_str());
    Value& uuids = uuid_doc["Items"];
    for(rapidjson::SizeType i = 0; i < uuids.Size();++i)
    {
        string id = uuids[i]["patent_id"]["S"].GetString();
       // cout << id << endl;
    }

 //   BSONBuilder buildObj;



//    string requestHeader  = string("host:") + dbHost + '\n';
 //   requestHeader += string("x-amz-date:") + getAmzDate() +'\n';
  //  requestHeader += string("x-amz-target:") + "DynamoDB_20120810.CreateTable" +"\n";

//    string requestStr = string("AWS4-HMAC-SHA256") + string("GET") +

    //HTTPRequest request(HTTPRequest::HTTP_POST, url.getPath(), HTTPRequest::HTTP_1_1);
}

