#!/bin/bash
echo 'step-1: freeze api... (you need execute as ". ./generate_aidl_api.sh" or "source ./generate_aidl_api.sh")'
m vendor.hidenorly.hardware.example-freeze-api
echo if the build success, do the following steps
echo step-2: cd aidl_api;../generate_aidl_hash.sh
echo step-3: . ../update_aidl_api.sh