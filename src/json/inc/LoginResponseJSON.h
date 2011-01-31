#ifndef LOGINRESPONSEJSON_H
#define LOGINRESPONSEJSON_H

#include "JsonSerializer.h"

class LoginResponseJSON : public JsonSerializer
{
public:
    LoginResponseJSON();

    QByteArray getJson() const;

    void parseJson(const QByteArray&);
};

#endif // LOGINRESPONSEJSON_H