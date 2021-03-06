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


#include <fstream>


#include "base/io/json/Json.h"
#include "3rdparty/rapidjson/document.h"
#include "3rdparty/rapidjson/istreamwrapper.h"
#include "3rdparty/rapidjson/ostreamwrapper.h"
#include "3rdparty/rapidjson/prettywriter.h"


bool superhack::Json::get(const char *fileName, rapidjson::Document &doc)
{
    std::ifstream ifs(fileName, std::ios_base::in | std::ios_base::binary);
    if (!ifs.is_open()) {
        return false;
    }

    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream<rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag>(isw);

    return !doc.HasParseError() && doc.IsObject();
}


bool superhack::Json::save(const char *fileName, const rapidjson::Document &doc)
{
    std::ofstream ofs(fileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!ofs.is_open()) {
        return false;
    }

    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

    doc.Accept(writer);

    return true;
}


bool superhack::Json::convertOffset(const char* fileName, size_t offset, size_t& line, size_t& pos, std::vector<std::string>& s)
{
    std::ifstream ifs(fileName, std::ios_base::in | std::ios_base::binary);
    if (!ifs.is_open()) {
        return false;
    }

    return convertOffset(ifs, offset, line, pos, s);
}
