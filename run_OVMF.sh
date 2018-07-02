#!/bin/sh
# Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
make run BIOS=$(find /usr/share -name OVMF_CODE.fd)
