#include <Environment.h>

Environment::Environment()
{
    this->HOST = this->getEnv(BAKUP_HOST, "bakup.io");
    this->INSECURE_PROTOCOL = this->getEnv(BAKUP_INSECURE_PROTOCOL, "http://");
    this->SECURE_PROTOCOL = this->getEnv(BAKUP_SECURE_PROTOCOL, "https://");
}

Environment::Environment(const Environment &environment)
{
    this->HOST = environment.HOST;
    this->INSECURE_PROTOCOL = environment.INSECURE_PROTOCOL;
    this->SECURE_PROTOCOL = environment.SECURE_PROTOCOL;
}

std::string Environment::getEnv(string env, string defaultValue)
{
    if (env == "")
    {
        return defaultValue;
    }

    return env;
}