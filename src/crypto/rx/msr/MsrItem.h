/* superhack
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2019 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2018-2019 tevador     <tevador@gmail.com>
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

#ifndef superhack_MSRITEM_H
#define superhack_MSRITEM_H


#include "base/tools/String.h"


#include <limits>
#include <vector>


namespace superhack
{


class RxDataset;


class MsrItem
{
public:
    constexpr static uint64_t kNoMask = std::numeric_limits<uint64_t>::max();

    inline MsrItem() = default;
    inline MsrItem(uint32_t reg, uint64_t value, uint64_t mask = kNoMask) : m_reg(reg), m_value(value), m_mask(mask) {}

    MsrItem(const rapidjson::Value &value);

    inline bool isValid() const     { return m_reg > 0; }
    inline uint32_t reg() const     { return m_reg; }
    inline uint64_t value() const   { return m_value; }
    inline uint64_t mask() const    { return m_mask; }

    rapidjson::Value toJSON(rapidjson::Document &doc) const;
    String toString() const;

private:
    uint32_t m_reg      = 0;
    uint64_t m_value    = 0;
    uint64_t m_mask     = kNoMask;
};


using MsrItems = std::vector<MsrItem>;


} /* namespace superhack */


#endif /* superhack_MSRITEM_H */
