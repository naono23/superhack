/* superhack
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

#ifndef superhack_NETBUFFER_H
#define superhack_NETBUFFER_H


struct uv_buf_t;
using uv_handle_t = struct uv_handle_s;


#include <cstddef>


namespace superhack {


class NetBuffer
{
public:
    static constexpr size_t kChunkSize = 16 * 1024;

    static char *allocate();
    static void destroy();
    static void onAlloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void release(const char *buf);
    static void release(const uv_buf_t *buf);
};


} /* namespace superhack */


#endif /* superhack_NETBUFFER_H */
