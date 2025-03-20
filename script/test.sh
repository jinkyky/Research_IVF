#!/bin/bash

#MAX_THREAD=?
USE_PCA=${USE_PCA:-0} #0: NO_PCA 1: ONLY_PCA 2:BOTH
ITER=${ITER:-100}
UPSTREAM="$(realpath "$(dirname "${BASH_SOURCE[0]}")/..")"
SRC_DIR="$UPSTREAM/src"   # 假设 ex_main 在 test.sh 的上级目录
OUTPUT_DIR="$UPSTREAM/output"
TESTED=${TESTED:-"ex_main"}

args=()

#根据是否定义USE_PCA MAX_THREAD生成测试的参数列表
for use_pca in 0 1; do
    if [ $use_pca -eq 0 ] && [ $USE_PCA -eq 1 ]; then
        continue
    fi
    if [ $use_pca -eq 1 ] && [ $USE_PCA -eq 0 ]; then
        continue
    fi
    if [ -z "$MAX_THREAD" ]; then #未指定最大线程数，则测试全部线程
        if [ $use_pca -eq 1 ]; then #当前参数选择pca
            for thread in 32 16 12 8 4; do
                args+=("--use-pca --limit-thread=$thread --iter=$ITER ")
            done
        else #当前参数不选择pca
            for thread in 32 16 12 8 4; do
                args+=("--limit-thread=$thread --iter=$ITER ")
            done
        fi
    else #指定了最大线程数，只测试一个线程
        if [ $use_pca -eq 1 ]; then
            args+=("--use-pca --limit-thread=$MAX_THREAD --iter=$ITER ")
        else
            args+=("--limit-thread=$MAX_THREAD --iter=$ITER ")
        fi
    fi
done

#根据参数列表执行多次被测试程序：ex_main
cd $UPSTREAM || exit 1
mkdir -p output
for i in "${!args[@]}"; do
    echo "executing ./$TESTED ${args[i]}"
    ./$TESTED ${args[i]} >> "$OUTPUT_DIR/test1M_$i" 2>&1
    if [ $? -ne 0 ]; then
        echo "Error executing ./$TESTED ${args[i]}" >> $OUTPUT_DIR/test1M_${i}_error
    fi
done

