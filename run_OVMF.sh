#!/bin/sh
make run BIOS=$(find /usr/share -name OVMF_CODE.fd)
