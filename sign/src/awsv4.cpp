#include "awsv4.hpp"

namespace AWSV4 {

    const std::string join(const std::vector<std::string>& ss,const std::string delim)  {
        std::stringstream sstream;
        const size_t l = ss.size() - 1;
        std::vector<int>::size_type i;
        for (i = 0; i < l; i++) {
            sstream << ss.at(i) << delim;
        }
        sstream << ss.back();
        return sstream.str();
    }

    // http://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    void sha256(const std::string str, unsigned char outputBuffer[SHA256_DIGEST_LENGTH])  {
        char *c_string = new char [str.length()+1];
        std::strcpy(c_string, str.c_str());        
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, c_string, strlen(c_string));
        SHA256_Final(hash, &sha256);
        for (int i=0;i<SHA256_DIGEST_LENGTH;i++) {
            outputBuffer[i] = hash[i];
        }
    }
    
    const std::string sha256_base16(const std::string str)  { 
        unsigned char hashOut[SHA256_DIGEST_LENGTH];
        AWSV4::sha256(str,hashOut);
        char outputBuffer[65];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(outputBuffer + (i * 2), "%02x", hashOut[i]);
        }
        outputBuffer[64] = 0;
        return std::string(outputBuffer);
    }

    // -----------------------------------------------------------------------------------
    // TASK 1 - create a canonical request
    // http://docs.aws.amazon.com/general/latest/gr/sigv4-create-canonical-request.html

    // uri should be normalize()'d before calling here, as this takes a const ref param and we don't 
    // want to normalize repeatedly. the return value is not a uri specifically, but a uri fragment,
    // as such the return value should not be used to initialize a uri object








    // -----------------------------------------------------------------------------------
    // TASK 3
    // http://docs.aws.amazon.com/general/latest/gr/sigv4-calculate-signature.html

    const std::string calculate_signature(const std::string request_date,
                                          const std::string secret,
                                          const std::string region,
                                          const std::string service,
                                          const std::string string_to_sign)  {

        const std::string k1(AWS4 + secret);
        char *c_k1 = new char [k1.length()+1];
        std::strcpy(c_k1, k1.c_str());

       // auto yyyymmdd = utc_yyyymmdd(request_date);
        char *c_yyyymmdd = new char [request_date.length()+1];
        std::strcpy(c_yyyymmdd, request_date.c_str());

        unsigned char* kDate;
        kDate = HMAC(EVP_sha256(), c_k1, strlen(c_k1), 
                     (unsigned char*)c_yyyymmdd, strlen(c_yyyymmdd), NULL, NULL); 

        char *c_region = new char [region.length()+1];
        std::strcpy(c_region, region.c_str());        
        unsigned char *kRegion;
        kRegion = HMAC(EVP_sha256(), kDate, strlen((char *)kDate), 
                     (unsigned char*)c_region, strlen(c_region), NULL, NULL); 

        char *c_service = new char [service.length()+1];
        std::strcpy(c_service, service.c_str());        
        unsigned char *kService;
        kService = HMAC(EVP_sha256(), kRegion, strlen((char *)kRegion), 
                     (unsigned char*)c_service, strlen(c_service), NULL, NULL); 

        char *c_aws4_request = new char [AWS4_REQUEST.length()+1];
        std::strcpy(c_aws4_request, AWS4_REQUEST.c_str());        
        unsigned char *kSigning;
        kSigning = HMAC(EVP_sha256(), kService, strlen((char *)kService), 
                     (unsigned char*)c_aws4_request, strlen(c_aws4_request), NULL, NULL); 

        char *c_string_to_sign = new char [string_to_sign.length()+1];
        std::strcpy(c_string_to_sign, string_to_sign.c_str());        
        unsigned char *kSig;
        kSig = HMAC(EVP_sha256(), kSigning, strlen((char *)kSigning), 
                     (unsigned char*)c_string_to_sign, strlen(c_string_to_sign), NULL, NULL); 

        char outputBuffer[65];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(outputBuffer + (i * 2), "%02x", kSig[i]);
        }
        outputBuffer[64] = 0;
        return std::string(outputBuffer);
    }
}
