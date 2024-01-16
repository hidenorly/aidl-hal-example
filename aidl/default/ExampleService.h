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
#pragma once

#include <aidl/vendor/hidenorly/hardware/example/BnExampleService.h>

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

class ExampleService : public BnExampleService {
public:
    void registerCallback(const android::sp<IExampleCallback>& callback);
    void unregisterCallback(const android::sp<IExampleCallback>& callback);


protected:
    void linkToDeathCallback(const android::sp<IExampleCallback>& callback);
    void notifyToClients();

private:
    struct CallbackInfo {
        android::sp<IExampleCallback> callback;
        android::binder_cookie_t cookie;
    };

    android::Mutex mCallbackLock;
    android::Vector<CallbackInfo> mCallbacks;
};

} // example
} // hardware
} // hidenorly
} // vendor
} // aidl
