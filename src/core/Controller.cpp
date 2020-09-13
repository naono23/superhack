/* superhack
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
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


#include "core/Controller.h"
#include "backend/cpu/Cpu.h"
#include "core/config/Config.h"
#include "core/Miner.h"
#include "crypto/common/VirtualMemory.h"
#include "net/Network.h"


#include <cassert>


superhack::Controller::Controller(Process *process) :
    Base(process)
{
}


superhack::Controller::~Controller()
{
    delete m_network;

    VirtualMemory::destroy();
}


int superhack::Controller::init()
{
    Base::init();

    VirtualMemory::init(config()->cpu().memPoolSize(), config()->cpu().isHugePages());

    m_network = new Network(this);

    return 0;
}


void superhack::Controller::start()
{
    Base::start();

    m_miner = new Miner(this);

    network()->connect();
}


void superhack::Controller::stop()
{
    Base::stop();

    delete m_network;
    m_network = nullptr;

    m_miner->stop();

    delete m_miner;
    m_miner = nullptr;
}


superhack::Miner *superhack::Controller::miner() const
{
    assert(m_miner != nullptr);

    return m_miner;
}


superhack::Network *superhack::Controller::network() const
{
    assert(m_network != nullptr);

    return m_network;
}


void superhack::Controller::execCommand(char command)
{
    miner()->execCommand(command);
    network()->execCommand(command);
}
