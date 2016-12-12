#include "signature.hpp"

void AWS4HttpAction::sha256(const std::string str, unsigned char outputBuffer[SHA256_DIGEST_LENGTH])
 {
        char *c_string = new char [str.length()+1];
        std::strcpy(c_string, str.c_str());
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, c_string, strlen(c_string));
        SHA256_Final(hash, &sha256);
        for (int i=0;i<SHA256_DIGEST_LENGTH;i++)
        {
            outputBuffer[i] = hash[i];
        }
}

string AWS4HttpAction::sha256_base16(string str)
{
        unsigned char hashOut[SHA256_DIGEST_LENGTH];
        sha256(str,hashOut);
        char outputBuffer[65];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(outputBuffer + (i * 2), "%02x", hashOut[i]);
        }
        outputBuffer[64] = 0;
        return std::string(outputBuffer);
}

void AWS4HttpAction::SetHttpBody(string &http_body)
{
	_http_body =  http_body;

}
void AWS4HttpAction::SetSignature()
{
        const std::string k1(_aws4 + _secret);
        char *c_k1 = new char [k1.length()+1];
        std::strcpy(c_k1, k1.c_str());


        char *c_yyyymmdd = new char [_amz_stamp.length()+1];
        strcpy(c_yyyymmdd, _amz_stamp.c_str());

        unsigned char* kDate;
        kDate = HMAC(EVP_sha256(), c_k1, strlen(c_k1),
                     (unsigned char*)c_yyyymmdd, strlen(c_yyyymmdd), NULL, NULL);

        char *c_region = new char [_region.length()+1];
        strcpy(c_region, _region.c_str());
        unsigned char *kRegion;
        kRegion = HMAC(EVP_sha256(), kDate, strlen((char *)kDate),
                     (unsigned char*)c_region, strlen(c_region), NULL, NULL);

        char *c_service = new char [_service.length()+1];
        std::strcpy(c_service, _service.c_str());
        unsigned char *kService;
        kService = HMAC(EVP_sha256(), kRegion, strlen((char *)kRegion),
                     (unsigned char*)c_service, strlen(c_service), NULL, NULL);

        char *c_aws4_request = new char [_aws4_request.length()+1];
        std::strcpy(c_aws4_request, _aws4_request.c_str());
        unsigned char *kSigning;
        kSigning = HMAC(EVP_sha256(), kService, strlen((char *)kService),
                     (unsigned char*)c_aws4_request, strlen(c_aws4_request), NULL, NULL);

        char *c_string_to_sign = new char [_string_to_sign.length()+1];
        std::strcpy(c_string_to_sign, _string_to_sign.c_str());
        unsigned char *kSig;
        kSig = HMAC(EVP_sha256(), kSigning, strlen((char *)kSigning),
                     (unsigned char*)c_string_to_sign, strlen(c_string_to_sign), NULL, NULL);

        char outputBuffer[65];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(outputBuffer + (i * 2), "%02x", kSig[i]);
        }
        outputBuffer[64] = 0;

        _signature = string(outputBuffer);

}

void AWS4HttpAction::SetCannocialHeader()
{
//	_amz_date = "20151212Z0922T";
//	_amz_stamp = "20151212";
	_amz_date  = GetAmzDate();
	_amz_stamp = GetDateStamp(_amz_date);
	_canonical_header.clear();
	_canonical_header   += "content-type:" + _content_type + _ENDL + "host:" + _host + _ENDL + \
			                "x-amz-date:"  + _amz_date     + _ENDL + "x-amz-target:" + _amz_target + _ENDL;
}

void AWS4HttpAction::SetSignedHeaders()
{
	_signed_headers.clear();
	_signed_headers += "content-type;host;x-amz-date;x-amz-target";
}

void AWS4HttpAction::SetPayloadHash()
{
	_payload_hash = sha256_base16(_http_body);
}

void AWS4HttpAction::SetCanonicalRequest()
{
	_canonical_request.clear();
	_canonical_request += _method + _ENDL + _uri + _ENDL + _query_string + _ENDL + \
			              _canonical_header      + _ENDL + _signed_headers + _ENDL + \
			              _payload_hash;
}

void AWS4HttpAction::SetCanonicalRequestHash()
{
	_canonical_request_hash  = sha256_base16(_canonical_request);
}

void AWS4HttpAction::SetCredentialScope()
{

	_credential_scope.clear();
	_credential_scope += _amz_stamp + _separator +  _region + _separator +
			             _service   + _separator +  _aws4_request;
}

void AWS4HttpAction::SetStringToSign()
{
	_string_to_sign.clear();
	_string_to_sign += _algorithm + _ENDL + _amz_date + _ENDL + _credential_scope + _ENDL + _canonical_request_hash;
}

void AWS4HttpAction::SetAuthorizationHeader()
{
	SetCannocialHeader();
	SetSignedHeaders();
	SetPayloadHash();
	SetCanonicalRequest();
	SetCanonicalRequestHash();
	SetCredentialScope();
	SetStringToSign();
	SetSignature();

	_authorization_header = _algorithm + " " + _amz_date + "Credential="  + _access_key + _separator +
			                _credential_scope + "," + "SignedHeaders=" + _signed_headers + ", " + "Signature=" + _signature;
}


void AWS4HttpAction::SetSession()
{
    _session.setHost(_host);
    _session.setPort(_port);
}

void AWS4HttpAction::SetReuestHeader()
{
	SetAuthorizationHeader();

    _request.setVersion(HTTPRequest::HTTP_1_1);
    _request.setURI(_uri.c_str());
   // _request.setMethod(HTTPRequest::HTTP_POST);
    _request.setMethod(_method);
    _request.setContentLength(_http_body.length());
    _request.set("X-Amz-Date", _amz_date.c_str());
    _request.set("X-Amz-Target", _amz_target.c_str());
    _request.set("Authorization", _authorization_header.c_str());

}
int  AWS4HttpAction::GetResponse(string &rlt)
{
	    SetReuestHeader();
	    ostream & os =  _session.sendRequest(_request);
	    os << _http_body;

	    HTTPResponse res;
	    istream &is = _session.receiveResponse(res);

	    int statusCode = (int)res.getStatus();
	    if(200 != statusCode)
	    {

	        copy((istreambuf_iterator<char>(is)), istreambuf_iterator<char>(),back_insert_iterator<string>(rlt));
	        rlt += res.getReason();

	    }
	    else
	    {
	    	copy((istreambuf_iterator<char>(is)), istreambuf_iterator<char>(),back_insert_iterator<string>(rlt));

	    }
	    return statusCode;
}

void AWS4HttpAction::display()
{
		cout << "canonical_header: " << endl;
		cout << _canonical_header << endl;
		cout << _payload_hash << endl;
		cout << _canonical_request << endl;
}
