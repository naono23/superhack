/* superhack
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2018-2019 SChernykh   <https://github.com/SChernykh>
 * Copyright 2018-2019 tevador     <tevador@gmail.com>
 * Copyright 2016-2019 superhack       <https://github.com/superhack>, <support@superhack.com>
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

#ifndef superhack_IMEMORYPOOL_H
#define superhack_IMEMORYPOOL_H


#include <cstddef>
#include <cstdint>


namespace superhack {


class IMemoryPool
{
public:
    virtual ~IMemoryPool() = default;

    virtual bool isHugePages(uint32_t node) const       = 0;
    virtual uint8_t *get(size_t size, uint32_t node)    = 0;
    virtual void release(uint32_t node)                 = 0;
};


} /* namespace superhack */



#endif /* superhack_IMEMORYPOOL_H */
