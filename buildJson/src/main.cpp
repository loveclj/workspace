
#include "common.hpp"
#include "buildJson.hpp"

using namespace JSONAPI;

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
	 Document doc;
     doc.SetObject();
	 Document::AllocatorType & allocator = doc.GetAllocator();

	 doc.AddMember("TableName", "patent", allocator);
	 Value v;
	 v.SetObject();
	 doc.AddMember("ProjectionExpression","", allocator);

	 vector<string> fields;
	 fields.push_back("a");
	 fields.push_back("b");
	 fields.push_back("c");
#if 0
	 map<string, string> kv;
	 kv["first"] = "1";
	 kv["second"] = "2";
#else
	 map<string, int> kv;
			 kv["first"] = 1;
			 kv["second"] = 2;
#endif

	 JsonAddObjByMap<Document>(doc, kv, allocator,"tes","s",ENDFIELD);


	 vector<string> vs;
	 vs.push_back("st");
	 vs.push_back("st2");

	 JsonAddArrayByVector<Document>(doc,vs,allocator,"abc","edf","\n");
	 JsonAddArrayByVector<Document>(doc,vs,allocator,"abc","edf","\n");
	 //doc["ProjectionExpression"].SetString("te");
	// doc["ProjectionExpression"].AddMember("a","b",allocator);
	 cout <<  doc2str(doc) << endl;

}
