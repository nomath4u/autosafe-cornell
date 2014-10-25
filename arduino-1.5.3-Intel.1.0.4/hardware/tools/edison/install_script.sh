#!/bin/bash

absolute_sdk_dir=$(readlink -f "$0")
absolute_sdk_dir=`dirname "$absolute_sdk_dir"`
cd "$absolute_sdk_dir"

target_sdk_dir=$(pwd)

if [ ! -e "$target_sdk_dir/sysroots" ]; then
printf "Extracting SDK (this will take a while)..."
$target_sdk_dir/toolchain*.sh -y -d $target_sdk_dir
#rm $target_sdk_dir/toolchain*.sh
echo "done"
fi

exit 0
