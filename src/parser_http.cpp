#include <algorithm>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <openssl/hmac.h>
#include <locale>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <request.h>
#include <response.h>
#include <parser_http.h>
#include <http.h>

using namespace std;

Request * ParserHTTP::getRequestFromStr(string & str) {
    size_t methodEndPos = str.find(' ');
    if (methodEndPos == string::npos) return new Request();
    string methodStr = str.substr(0, methodEndPos);
    HTTP::Method method = HTTP::getMethod(methodStr);

    size_t uriEndPos = str.find(' ', methodEndPos + 1);
    if (uriEndPos == string::npos) return new Request();
    string uriStr = str.substr(methodEndPos + 1, uriEndPos - methodEndPos - 1);

    size_t versionEndPos = str.find("\r\n", uriEndPos + 1);
    if (versionEndPos == string::npos) return new Request();
    string versionStr = str.substr(uriEndPos + 1, versionEndPos - uriEndPos - 1);
    HTTP::Version version = HTTP::getVersion(versionStr);

    size_t headersEndPos = str.find("\r\n\r\n", versionEndPos + 2);
    if (headersEndPos == string::npos) return new Request();
    string headersStr = str.substr(versionEndPos + 2, headersEndPos - versionEndPos - 2);

    string bodyStr = str.substr(headersEndPos + 4);
    return new Request(method, uriStr, version, headersStr, bodyStr, str);
}

string ParserHTTP::getStrFromResponse(Response & response) {
    string version = HTTP::getVersion(response.getVersion());
    string status = std::to_string(response.getStatus());
    string reasonPhrase = HTTP::getReasonPhrase(response.getStatus());
    string body = response.getBody()->getBody();
    response.getHeaders()->add("Content-length", std::to_string(body.length()).c_str());
    response.getHeaders()->add("Connection", "close");
    string headers = response.getHeaders()->toString();
    return version + " " + status + " " + reasonPhrase + "\r\n" + headers + "\r\n\r\n" + body;
}

static char reservedSymbols[] {'/', '-', '_', '.', '~', ';', '?', ':', '@', '&', '=', '+',
                                '!', '*', '(', ')', ',', '$', '#'};

static bool containsArr(const char * symbols, int len_arr, char c) {
    for (int i = 0; i < len_arr; i++) {
        if (symbols[i] == c) return true;
    }
    return false;
}

string ParserHTTP::urlEncode(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;
    for (auto & c : value) {
        if (isalnum(c) || containsArr(reservedSymbols, sizeof(reservedSymbols) / sizeof(char), c)) {
            escaped << c;
        } else {
            escaped << uppercase;
            escaped << '%' << setw(2) << int((unsigned char) c);
            escaped << nouppercase;
        }
    }
    return escaped.str();
}

string ParserHTTP::urlDecode(const string & value) {
    ostringstream result;
    for (size_t i = 0; i < value.length(); i++) {
        string::value_type c = value[i];
        if (c == '+') {
            result << ' ';
        } else if (isalnum(c) || containsArr(reservedSymbols, sizeof(reservedSymbols) / sizeof(char), c)) {
            result << c;
        } else if (c == '%' && i + 2 < value.length()) {
            string hex = string();
            if (!isxdigit(value[i + 1]) || !isxdigit(value[i + 2])) {
                return value;
            }
            hex += value[++i];
            hex += value[++i];
            auto cur = (unsigned char) stoi(hex, nullptr, 16);
            result << cur;
        } else {
            return value;
        }
    }
    return result.str();
}

std::string ParserHTTP::getTime(const time_t * time_struct, const char * format) {
    time_t raw_time = time_struct ? *time_struct : time(&raw_time);
    tm * time_cur;
    time_cur = localtime(&raw_time);
    char time_str[1024];
    strftime(time_str, sizeof(time_str), format, time_cur);
    std::string res{time_str};
    return res;
}

// https://stackoverflow.com/a/217605/9431509
// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim_both(std::string &s) {
    ltrim(s);
    rtrim(s);
}

string ParserHTTP::trim(string &str) {
    string res = str;
    trim_both(res);
    return res;
}

std::string ParserHTTP::sha256(std::string &key, std::string & message) {
    unsigned char hash[32];
    HMAC_CTX *hmac = HMAC_CTX_new();
    HMAC_Init_ex(hmac, &key[0], key.length(), EVP_sha256(), nullptr);
    HMAC_Update(hmac, ( unsigned char* )&message[0], message.length());
    unsigned int len = 32;
    HMAC_Final(hmac, hash, &len);
    std::stringstream ss;
    ss << std::setfill('0');
    for (int i = 0; i < len; i++) {
        ss << hex << ( unsigned int ) hash[i];
    }
    HMAC_CTX_free(hmac);
    return (ss.str());
}


// https://github.com/ReneNyffenegger/cpp-base64

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string ParserHTTP::base64Encode(std::string & value) {
    auto bytes_to_encode = reinterpret_cast<const unsigned char*>(value.c_str());
    unsigned int in_len = value.length();

    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string ParserHTTP::base64Decode(std::string & encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}
