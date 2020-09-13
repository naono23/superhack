/* superhack
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018-2020 SChernykh   <https://github.com/SChernykh>
 * Copyright 2016-2020 superhack       <https://github.com/superhack>, <support@superhack.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef superhack_ICLIENTLISTENER_H
#define superhack_ICLIENTLISTENER_H


#include <cstdint>


#include "3rdparty/rapidjson/fwd.h"


namespace superhack {


class Algorithm;
class IClient;
class Job;
class SubmitResult;


class IClientListener
{
public:
    virtual ~IClientListener() = default;

    virtual void onClose(IClient *client, int failures)                                           = 0;
    virtual void onJobReceived(IClient *client, const Job &job, const rapidjson::Value &params)   = 0;
    virtual void onLogin(IClient *client, rapidjson::Document &doc, rapidjson::Value &params)     = 0;
    virtual void onLoginSuccess(IClient *client)                                                  = 0;
    virtual void onResultAccepted(IClient *client, const SubmitResult &result, const char *error) = 0;
    virtual void onVerifyAlgorithm(const IClient *client, const Algorithm &algorithm, bool *ok)   = 0;
};


} /* namespace superhack */


#endif // superhack_ICLIENTLISTENER_H