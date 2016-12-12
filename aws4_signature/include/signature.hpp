#ifndef __SIGNATURE_HPP__
#define __SIGNATURE_HPP__

#include<iostream>
#include<string>
#include<time.h>
#include<sstream>
#include<fstream>
#include<map>
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

#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"
#include "Poco/HMACEngine.h"

#include "openssl/sha.h"
#include "openssl/hmac.h"

#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"

#include "amz_date.hpp"

using namespace Poco;
using namespace Poco::Net;

using namespace std;

class AWS4HttpAction
{
public:
	AWS4HttpAction(string ENDL,
			      string method,
			      string algorithm,
			      string aws4,
			      string aws4_request,
			      string host,
			      string separator,
			      string service,
			      string uri,
			      string region,
			      string content_type,
			      string key_file_path,
			      string amz_target,
			      Poco::UInt16 port = 80):  _ENDL(ENDL),
			                           _method(method),
			                           _algorithm(algorithm),
			                           _aws4(aws4),
			                           _aws4_request(aws4_request),
			                           _host(host),
			                           _separator(separator),
			                           _service(service),
			                           _uri(uri),
			                           _region(region),
			                           _content_type(content_type),
			                           _key_file_path(key_file_path),
			                           _amz_target(amz_target),
			                           _port(port)
   {
		/**** get key ****/
		fstream fs(_key_file_path.c_str());
		fs >> _access_key;
		fs >> _secret;
		fs.close();

		SetSession();

	}

	/* API */
	int  GetResponse(string &rlt); //send post request and get response
	void SetHttpBody(string &http_body); // set http body
	void display(); //check


private:

	void SetCannocialHeader();
	void SetSignedHeaders();
	void SetPayloadHash();
	void SetCanonicalRequest();
	void SetCanonicalRequestHash();
	void SetCredentialScope();
	void SetStringToSign();
	void SetSession();
	void SetReuestHeader();
	void SetAuthorizationHeader();

	void sha256(const std::string str, unsigned char outputBuffer[SHA256_DIGEST_LENGTH]) ;
	string sha256_base16(const std::string);
	void SetSignature();




private:
	string _ENDL;         // "\n"
	string _method;       // http action, HTTP,GET and so on
	string _algorithm;    // AWS4-HMAC-SHA256
	string _aws4;         // "AWS4"
	string _aws4_request; // aws4_request
	string _host;         //dynamodb ip
	string _separator;    // "/"

	string _service;      // dynamodb
	string _uri;          // "/"
	string _region;       // cn-north-1
	string _content_type; //application/x-amz-json-1.0
	string _key_file_path;
	string _amz_target;   //DynamoDB_20120810.Scan

	string _query_string; // query string in http header
	string _amz_date;     // iso8061 formate, yyyymmddThhmmssZ
	string _amz_stamp;    // yyyymmdd

	string _http_body;

	string _access_key;
	string _secret;
	/*
	 * string to compute signature
	 */
	string _canonical_header;
	string _payload_hash;
	string _signed_headers;
	string _canonical_request;
	string _canonical_request_hash;
	string _credential_scope;
	string _string_to_sign;
	string _signature;
	string _authorization_header;


	HTTPClientSession _session;
	HTTPRequest _request;
	Poco::UInt16 _port;

};



#endif //end __SIGNATURE_HPP__
