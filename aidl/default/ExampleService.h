#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

using android::OK;
using android::sp;
using android::status_t;
using android::String16;
using android::IInterface;
using android::IBinder;
using android::IPCThreadState;
using android::ProcessState;
using android::BnDeathRecipient;


class ExampleService : public android::BBinder {
public:
    static void instantiate();

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