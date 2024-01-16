/*
  Copyright (C) 2024 hidenorly

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include "ExampleService.h"

using android::OK;
using android::sp;
using android::status_t;
using android::String16;
using android::IInterface;
using android::IBinder;
using android::IPCThreadState;
using android::ProcessState;
using android::BnDeathRecipient;

namespace aidl {
namespace vendor {
namespace hidenorly {
namespace hardware {
namespace example {

void ExampleService::registerCallback(const android::sp<IExampleCallback>& callback) {
    android::Mutex::Autolock lock(mCallbackLock);

    android::binder_cookie_t cookie = android::IPCThreadState::self()->newCookie();
    android::IInterface::asBinder(callback)->linkToDeath(new android::BnDeathRecipient(callback, cookie), 0);

    CallbackInfo callbackInfo = {callback, cookie};
    mCallbacks.push(callbackInfo);
}

void ExampleService::unregisterCallback(const android::sp<IExampleCallback>& callback) {
    android::Mutex::Autolock lock(mCallbackLock);

    auto callbackIt = std::find_if(mCallbacks.begin(), mCallbacks.end(),
        [callback](const CallbackInfo& info) {
            return callback == info.callback;
        });

    if (callbackIt != mCallbacks.end()) {
        android::IInterface::asBinder(callback)->unlinkToDeath(new android::BnDeathRecipient(callback, callbackIt->cookie)).clear();
        mCallbacks.erase(callbackIt);
    }
}

void ExampleService::linkToDeathCallback(const android::sp<IExampleCallback>& callback) {
    if (callback != nullptr) {
        android::IInterface::asBinder(callback)->linkToDeath(new android::BnDeathRecipient(callback), 0);
    }
}

void ExampleService::notifyToClients() {
    android::Mutex::Autolock lock(mCallbackLock);

    for (auto& aCallback : mCallbacks) {
        aCallback.callback->onCallback();
    }
}

} // example
} // hardware
} // hidenorly
} // vendor
} // aidl

using aidl::vendor::hidenoly::hardware::example::ExampleService;

int main() {
    // Instantiate ExampleService
    sp<ExampleService> exampleService = new ExampleService();

    // Register to Service Manager
    status_t status = android::defaultServiceManager()->addService(
        String16("vendor.hidenorly.hardware.example/default"), exampleService);

    if (status != OK) {
        LOG(ERROR) << "Failed to register service: " << status;
        return 1;
    }

    // Start IPC thread
    ProcessState::self()->startThreadPool();

    // Main loop
    IPCThreadState::self()->joinThreadPool();

    return 0;
}
