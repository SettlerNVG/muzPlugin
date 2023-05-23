/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== auth.php ==================
static const unsigned char temp_binary_data_0[] =
"<?php\n"
"function sendResponse() {\n"
"\n"
"    $response  = '<?xml version=\"1.0\" encoding=\"utf-8\"?>';\n"
"    if($_POST[\"email\"] === \"test@juce.com\" and $_POST[\"pw\"] === \"test\")\n"
"        $response .= '<MESSAGE message=\"Thanks for registering our product!\"><KEY>INSERT_KEY_HERE</KEY></MESSAGE>';\n"
"    else\n"
"        $response .= '<ERROR error=\"Sorry, we were not able to authorise your request. Please provide a valid email address and password.\"></ERROR>';\n"
"\n"
"    return $response;\n"
"}\n"
"\n"
"header(\"Content-type: text/xml; charset=utf-8\");\n"
"echo sendResponse();\n"
"?>\n";

const char* auth_php = (const char*) temp_binary_data_0;

//================== stunnel.conf ==================
static const unsigned char temp_binary_data_1[] =
"[https]\n"
"accept = 8443\n"
"connect = 8080\n"
"cert = stunnel.pem\n"
"TIMEOUTclose = 0\n";

const char* stunnel_conf = (const char*) temp_binary_data_1;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x5563aa61:  numBytes = 538; return auth_php;
        case 0x86b22248:  numBytes = 73; return stunnel_conf;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "auth_php",
    "stunnel_conf"
};

const char* originalFilenames[] =
{
    "auth.php",
    "stunnel.conf"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
