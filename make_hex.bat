@echo off
..\third_party\python_26\python.exe tools/make_hex_module.py --output-dir hex_binary_distrib --client --hexclient=1 \ %*
