#!/bin/bash
(find ./ -name "*.aidl" -print0 | LC_ALL=C sort -z | xargs -0 sha1sum && echo latest-version ) | sha1sum | cut -d " " -f 1 > .hash
cat .hash
ls -la
