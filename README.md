# superhack

[![Github All Releases](https://img.shields.io/github/downloads/superhack/superhack/total.svg)](https://github.com/superhack/superhack/releases)
[![GitHub release](https://img.shields.io/github/release/superhack/superhack/all.svg)](https://github.com/superhack/superhack/releases)
[![GitHub Release Date](https://img.shields.io/github/release-date/superhack/superhack.svg)](https://github.com/superhack/superhack/releases)
[![GitHub license](https://img.shields.io/github/license/superhack/superhack.svg)](https://github.com/superhack/superhack/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/superhack/superhack.svg)](https://github.com/superhack/superhack/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/superhack/superhack.svg)](https://github.com/superhack/superhack/network)

superhack is a high performance, open source, cross platform RandomX, KawPow, CryptoNight and AstroBWT unified CPU/GPU miner. Official binaries are available for Windows, Linux, macOS and FreeBSD.

## Mining backends
- **CPU** (x64/ARMv8)
- **OpenCL** for AMD GPUs.
- **CUDA** for NVIDIA GPUs via external [CUDA plugin](https://github.com/superhack/superhack-cuda).

## Download
* **[Binary releases](https://github.com/superhack/superhack/releases)**
* **[Build from source](https://superhack.com/docs/miner/build)**

## Usage
The preferred way to configure the miner is the [JSON config file](src/config.json) as it is more flexible and human friendly. The [command line interface](https://superhack.com/docs/miner/command-line-options) does not cover all features, such as mining profiles for different algorithms. Important options can be changed during runtime without miner restart by editing the config file or executing API calls.

* **[Wizard](https://superhack.com/wizard)** helps you create initial configuration for the miner.
* **[Workers](http://workers.superhack.info)** helps manage your miners via HTTP API.

## Donations
* Default donation 1% (1 minute in 100 minutes) can be increased via option `donate-level` or disabled in source code.
* XMR: `48edfHu7V9Z84YzzMa6fUueoELZ9ZRXq9VetWzYGzKt52XU5xvqgzYnDK9URnRoJMk1j8nLwEVsaSWJ4fhdUyZijBGUicoD`

## Developers
* **[superhack](https://github.com/superhack)**
* **[sech1](https://github.com/SChernykh)**

## Contacts
* support@superhack.com
* [reddit](https://www.reddit.com/user/superhack/)
* [twitter](https://twitter.com/superhack_dev)
