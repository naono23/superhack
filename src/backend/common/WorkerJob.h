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

#ifndef superhack_WORKERJOB_H
#define superhack_WORKERJOB_H


#include <cstring>


#include "base/net/stratum/Job.h"
#include "crypto/common/Nonce.h"


namespace superhack {


template<size_t N>
class WorkerJob
{
public:
    inline const Job &currentJob() const    { return m_jobs[index()]; }
    inline uint32_t *nonce(size_t i = 0)    { return reinterpret_cast<uint32_t*>(blob() + (i * currentJob().size()) + nonceOffset()); }
    inline uint64_t sequence() const        { return m_sequence; }
    inline uint8_t *blob()                  { return m_blobs[index()]; }
    inline uint8_t index() const            { return m_index; }


    inline void add(const Job &job, uint32_t reserveCount, Nonce::Backend backend)
    {
        m_sequence = Nonce::sequence(backend);

        if (currentJob() == job) {
            return;
        }

        if (index() == 1 && job.index() == 0 && job == m_jobs[0]) {
            m_index = 0;
            return;
        }

        save(job, reserveCount, backend);
    }


    inline bool nextRound(uint32_t rounds, uint32_t roundSize)
    {
        bool ok = true;
        m_rounds[index()]++;

        if ((m_rounds[index()] % rounds) == 0) {
            for (size_t i = 0; i < N; ++i) {
                *nonce(i) = Nonce::next(index(), *nonce(i), rounds * roundSize, currentJob().isNicehash(), &ok);
                if (!ok) {
                    break;
                }
            }
        }
        else {
            for (size_t i = 0; i < N; ++i) {
                *nonce(i) += roundSize;
            }
        }

        return ok;
    }


private:
    inline int32_t nonceOffset() const  { return currentJob().nonceOffset(); }
    inline size_t nonceSize() const     { return currentJob().nonceSize(); }

    inline void save(const Job &job, uint32_t reserveCount, Nonce::Backend backend)
    {
        m_index           = job.index();
        const size_t size = job.size();
        m_jobs[index()]   = job;
        m_rounds[index()] = 0;

        m_jobs[index()].setBackend(backend);

        for (size_t i = 0; i < N; ++i) {
            memcpy(m_blobs[index()] + (i * size), job.blob(), size);
            *nonce(i) = Nonce::next(index(), *nonce(i), reserveCount, job.isNicehash());
        }
    }


    alignas(16) uint8_t m_blobs[2][Job::kMaxBlobSize * N]{};
    Job m_jobs[2];
    uint32_t m_rounds[2] = { 0, 0 };
    uint64_t m_sequence  = 0;
    uint8_t m_index      = 0;
};


template<>
inline uint32_t *superhack::WorkerJob<1>::nonce(size_t)
{
    return reinterpret_cast<uint32_t*>(blob() + nonceOffset());
}


template<>
inline bool superhack::WorkerJob<1>::nextRound(uint32_t rounds, uint32_t roundSize)
{
    bool ok = true;
    m_rounds[index()]++;

    uint32_t* n = nonce();
    const uint32_t prev_nonce = *n;

    if ((m_rounds[index()] % rounds) == 0) {
        *n = Nonce::next(index(), *n, rounds * roundSize, currentJob().isNicehash(), &ok);
    }
    else {
        *n += roundSize;
    }

    // Increment higher 32 bits of a 64-bit nonce when lower 32 bits overflow
    if (!currentJob().isNicehash() && (nonceSize() == sizeof(uint64_t))) {
        const bool wrapped = (*n < prev_nonce);
        const bool wraps_this_round = (static_cast<uint64_t>(*n) + roundSize > (1ULL << 32));

        // Account for the case when starting nonce hasn't wrapped yet, but some nonces in the current round will wrap
        if (wrapped || wraps_this_round) {
            // Set lower 32 bits to 0 when higher 32 bits change
            Nonce::reset(index());

            // Sets *n to 0 and Nonce::m_nonce[index] to the correct next value
            *n = 0;
            Nonce::next(index(), *n, rounds * roundSize, currentJob().isNicehash(), &ok);

            ++n[1];

            Job& job = m_jobs[index()];
            memcpy(job.blob(), blob(), job.size());
        }
    }

    return ok;
}


template<>
inline void superhack::WorkerJob<1>::save(const Job &job, uint32_t reserveCount, Nonce::Backend backend)
{
    m_index           = job.index();
    m_jobs[index()]   = job;
    m_rounds[index()] = 0;

    m_jobs[index()].setBackend(backend);

    memcpy(blob(), job.blob(), job.size());
    *nonce() = Nonce::next(index(), *nonce(), reserveCount, currentJob().isNicehash());
}


} // namespace superhack


#endif /* superhack_WORKERJOB_H */
