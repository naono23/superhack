/* superhack
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2018-2019 SChernykh   <https://github.com/SChernykh>
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

#ifndef superhack_WORKERS_H
#define superhack_WORKERS_H


#include "backend/common/Thread.h"
#include "backend/cpu/CpuLaunchData.h"
#include "base/tools/Object.h"


#ifdef superhack_FEATURE_OPENCL
#   include "backend/opencl/OclLaunchData.h"
#endif


#ifdef superhack_FEATURE_CUDA
#   include "backend/cuda/CudaLaunchData.h"
#endif


namespace superhack {


class Hashrate;
class WorkersPrivate;
class Job;


template<class T>
class Workers
{
public:
    superhack_DISABLE_COPY_MOVE(Workers)

    Workers();
    ~Workers();

    const Hashrate *hashrate() const;
    void setBackend(IBackend *backend);
    void start(const std::vector<T> &data);
    void stop();
    void tick(uint64_t ticks);
    void jobEarlyNotification(const Job&);

private:
    static IWorker *create(Thread<T> *handle);
    static void onReady(void *arg);

    std::vector<Thread<T> *> m_workers;
    WorkersPrivate *d_ptr;
};


template<class T>
void superhack::Workers<T>::jobEarlyNotification(const Job& job)
{
    for (Thread<T>* t : m_workers) {
        if (t->worker()) {
            t->worker()->jobEarlyNotification(job);
        }
    }
}


template<>
IWorker *Workers<CpuLaunchData>::create(Thread<CpuLaunchData> *handle);
extern template class Workers<CpuLaunchData>;


#ifdef superhack_FEATURE_OPENCL
template<>
IWorker *Workers<OclLaunchData>::create(Thread<OclLaunchData> *handle);
extern template class Workers<OclLaunchData>;
#endif


#ifdef superhack_FEATURE_CUDA
template<>
IWorker *Workers<CudaLaunchData>::create(Thread<CudaLaunchData> *handle);
extern template class Workers<CudaLaunchData>;
#endif


} // namespace superhack


#endif /* superhack_WORKERS_H */
