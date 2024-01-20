# aidl-hal-example

This is an example of Android aidl hal.

You need to add the following into ```yourproduct.mk" in ```device/yourproject```:

```
PRODUCT_PACKAGES += vendor.hidenoly.hardware.example-service
```

And you need to move this files under ```vendor/hidenorly/hardware/aidl-hal-example```, etc.

And execute ```. generate_aidl_api.sh``` after ```source build/envsetup.sh; lunch yourproject``` if you need to fail.
