@echo off
..\third_party\python_26\python.exe tools/make_hex_module.py --output-dir hex_module_binary_distrib --hexclient=1 \ %*
