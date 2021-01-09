#include "SSLChecker.h"

SSLChecker::SSLChecker(cpr::ErrorCode errorCode) : errorCode(errorCode) {}

bool SSLChecker::checkSSLValid()
{
    switch (this->errorCode)
    {
        case cpr::ErrorCode::SSL_REMOTE_CERTIFICATE_ERROR:
        case cpr::ErrorCode::SSL_LOCAL_CERTIFICATE_ERROR:
        case cpr::ErrorCode::SSL_CONNECT_ERROR:
        case cpr::ErrorCode::SSL_CACERT_ERROR:
        case cpr::ErrorCode::GENERIC_SSL_ERROR:
            return false;
        default:
            return true;
    }
}