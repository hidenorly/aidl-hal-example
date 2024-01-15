// ExampleService.cpp

#include <android-base/logging.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include "IExampleService.h"

using android::OK;
using android::sp;
using android::status_t;
using android::String16;
using android::IInterface;
using android::IBinder;
using android::IPCThreadState;
using android::ProcessState;
using android::BnDeathRecipient;



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
        // デッド状態のCallbackを解除
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

void ExampleService::instantiate() {
    android::defaultServiceManager()->addService(android::String16("example.service"), new ExampleService());
}


int main() {
    // ExampleService インスタンスを作成
    sp<ExampleService> exampleService = new ExampleService();

    // Service Manager にサービスを登録
    status_t status = android::defaultServiceManager()->addService(
        String16("vendor.example.hardware.example_aidl_service"), exampleService);

    if (status != OK) {
        LOG(ERROR) << "Failed to register service: " << status;
        return 1;
    }

    // IPC スレッドを開始
    ProcessState::self()->startThreadPool();

    // Main loop
    IPCThreadState::self()->joinThreadPool();

    return 0;
}
