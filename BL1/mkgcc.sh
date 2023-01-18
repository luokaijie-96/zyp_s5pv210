#!/bin/bash
# 一键生成 arm-linux-gcc -> arm-none-linux-gnueabi-gcc 这类的工具链符号链接

cmd=`ls arm*`
prefix_symbol_name="arm-linux-"
for elem in $cmd
do
    suff=`echo $elem | awk -F'-' '{print $5}'`
    ln $elem -s ${prefix_symbol_name}$suff
done
