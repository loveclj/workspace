
#include "signature.hpp"

using namespace std;

using namespace rapidjson;
using namespace Poco::Net;

string  doc2str(Document &doc)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    string str(buffer.GetString(), buffer.GetSize());
    return str;

}

int main()
{

	string key_file_path = "conf/keys.text";
	string aws4("AWS4");
	string aws4_request("aws4_request");
	string separator = "/";
	string ENDL = "\n";
	string method("POST");
	string service("dynamodb");
	//string host("dynamodb.cn-north-1.amazonaws.com.cn");
	string host("192.168.6.170");
	string uri("/");
	string region("cn-north-1");
	string content_type("application/x-amz-json-1.0");
	string amz_target("DynamoDB_20120810.Scan");
	string query_string("");
	string algorithm("AWS4-HMAC-SHA256");

	AWS4HttpAction *aws_http = new AWS4HttpAction(ENDL, method, algorithm, aws4,
			aws4_request, host, separator, service, uri, region, content_type,
			key_file_path,amz_target,8000);

	 Document doc;
     doc.SetObject();
	 Document::AllocatorType & allocator = doc.GetAllocator();

	 doc.AddMember("TableName", "patent", allocator);
	 doc.AddMember("ProjectionExpression", "patent_id", allocator);


		 //   doc.AddMember("")
		string body = doc2str(doc);

	string http_body(body);
	aws_http->SetHttpBody(http_body);

	string reponse;
	aws_http->GetResponse(reponse);
	cout << reponse << endl;

	//aws_http.display();


}
