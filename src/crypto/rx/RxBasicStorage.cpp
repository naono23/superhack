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


#include "crypto/rx/RxBasicStorage.h"
#include "backend/common/Tags.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/tools/Chrono.h"
#include "base/tools/Object.h"
#include "crypto/rx/RxAlgo.h"
#include "crypto/rx/RxCache.h"
#include "crypto/rx/RxDataset.h"
#include "crypto/rx/RxSeed.h"


namespace superhack {


constexpr size_t oneMiB = 1024 * 1024;


class RxBasicStoragePrivate
{
public:
    superhack_DISABLE_COPY_MOVE(RxBasicStoragePrivate)

    inline RxBasicStoragePrivate() = default;
    inline ~RxBasicStoragePrivate() { deleteDataset(); }

    inline bool isReady(const Job &job) const   { return m_ready && m_seed == job; }
    inline RxDataset *dataset() const           { return m_dataset; }
    inline void deleteDataset()                 { delete m_dataset; m_dataset = nullptr; }


    inline void setSeed(const RxSeed &seed)
    {
        m_ready = false;

        if (m_seed.algorithm() != seed.algorithm()) {
            RxAlgo::apply(seed.algorithm());
        }

        m_seed = seed;
    }


    inline bool createDataset(bool hugePages, bool oneGbPages, RxConfig::Mode mode)
    {
        const uint64_t ts = Chrono::steadyMSecs();

        m_dataset = new RxDataset(hugePages, oneGbPages, true, mode, 0);
        if (!m_dataset->cache()->get()) {
            deleteDataset();

            LOG_INFO("%s" RED_BOLD("failed to allocate RandomX memory") BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);

            return false;
        }

        printAllocStatus(ts);

        return true;
    }


    inline void initDataset(uint32_t threads, int priority)
    {
        const uint64_t ts = Chrono::steadyMSecs();

        m_ready = m_dataset->init(m_seed.data(), threads, priority);

        if (m_ready) {
            LOG_INFO("%s" GREEN_BOLD("dataset ready") BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);
        }
    }


private:
    void printAllocStatus(uint64_t ts)
    {
        if (m_dataset->get() != nullptr) {
            const auto pages = m_dataset->hugePages();

            LOG_INFO("%s" GREEN_BOLD("allocated") CYAN_BOLD(" %zu MB") BLACK_BOLD(" (%zu+%zu)") " huge pages %s%1.0f%% %u/%u" CLEAR " %sJIT" BLACK_BOLD(" (%" PRIu64 " ms)"),
                     Tags::randomx(),
                     pages.size / oneMiB,
                     RxDataset::maxSize() / oneMiB,
                     RxCache::maxSize() / oneMiB,
                     (pages.isFullyAllocated() ? GREEN_BOLD_S : (pages.allocated == 0 ? RED_BOLD_S : YELLOW_BOLD_S)),
                     pages.percent(),
                     pages.allocated,
                     pages.total,
                     m_dataset->cache()->isJIT() ? GREEN_BOLD_S "+" : RED_BOLD_S "-",
                     Chrono::steadyMSecs() - ts
                     );
        }
        else {
            LOG_WARN(CLEAR "%s" YELLOW_BOLD_S "failed to allocate RandomX dataset, switching to slow mode" BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);
        }
    }


    bool m_ready         = false;
    RxDataset *m_dataset = nullptr;
    RxSeed m_seed;
};


} // namespace superhack


superhack::RxBasicStorage::RxBasicStorage() :
    d_ptr(new RxBasicStoragePrivate())
{
}


superhack::RxBasicStorage::~RxBasicStorage()
{
    delete d_ptr;
}


bool superhack::RxBasicStorage::isAllocated() const
{
    return d_ptr->dataset() && d_ptr->dataset()->cache() && d_ptr->dataset()->cache()->get();
}


superhack::HugePagesInfo superhack::RxBasicStorage::hugePages() const
{
    if (!d_ptr->dataset()) {
        return {};
    }

    return d_ptr->dataset()->hugePages();
}


superhack::RxDataset *superhack::RxBasicStorage::dataset(const Job &job, uint32_t) const
{
    if (!d_ptr->isReady(job)) {
        return nullptr;
    }

    return d_ptr->dataset();
}


void superhack::RxBasicStorage::init(const RxSeed &seed, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority)
{
    d_ptr->setSeed(seed);

    if (!d_ptr->dataset() && !d_ptr->createDataset(hugePages, oneGbPages, mode)) {
        return;
    }

    d_ptr->initDataset(threads, priority);
}