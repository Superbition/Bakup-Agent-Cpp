#ifndef BAKUP_AGENT_ENVIRONMENT_H
#define BAKUP_AGENT_ENVIRONMENT_H

#ifndef BAKUP_HOST
    #define BAKUP_HOST ""
#endif

#ifndef BAKUP_INSECURE_PROTOCOL
    #define BAKUP_INSECURE_PROTOCOL ""
#endif

#ifndef BAKUP_SECURE_PROTOCOL
    #define BAKUP_SECURE_PROTOCOL ""
#endif

#include <string>

using namespace std;

class Environment
{
    public:
        // Constructor
        Environment();

        // Copy constructor
        Environment(const Environment &environment);

        // Function returns environment variable or default value if empty
        string getEnv(string env, string defaultValue);

        string HOST;

        string INSECURE_PROTOCOL;

        string SECURE_PROTOCOL;
};

#endif //BAKUP_AGENT_ENVIRONMENT_H