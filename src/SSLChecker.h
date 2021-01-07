#ifndef BAKUP_AGENT_SSLCHECKER_H
#define BAKUP_AGENT_SSLCHECKER_H

#include <cpr/error.h>

class SSLChecker {
private:
    // Store the error code
    cpr::ErrorCode errorCode;

public:
    // Generate the SSLChecker instance with an error code
    SSLChecker(cpr::ErrorCode errorCode);

    // Check if the error is an SSL error
    bool checkSSLValid();
};


#endif //BAKUP_AGENT_SSLCHECKER_H