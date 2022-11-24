# 用 Qt 做一个用 Paddle Inference 进行图像分割推理的图形界面程序

## 参考资料

### Paddle Inference 与 OpenCV 的 gflags 冲突

一开始引入 OpenCV 时可以编译通过，但在运行时会出现错误：

```
ERROR: flag 'flagfile' was defined more than once (in files '/Users/paddle/xly/workspace/com_infer/Paddle/build/third_party/gflags/src/extern_gflags/src/gflags.cc' and '/tmp/gflags-20201222-58611-19juerb/gflags-2.2.2/src/gflags.cc').
```

查了一下别人也有类似的错误：
- https://aistudio.baidu.com/paddle/forum/topic/show/957985
- https://github.com/PaddlePaddle/Paddle/issues/44470

在自己重新编译了 Paddle Inference 和 OpenCV 后问题得到解决。下面列出针对 M1 芯片的 Mac 的编译方式。

#### 编译 OpenCV

```bash
cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_ZLIB=OFF -D CMAKE_INSTALL_PREFIX=/usr/local/opencv ..
make -j$(nproc)
make install
```

#### 编译 Paddle Inference

```bash
git clone https://github.com/PaddlePaddle/Paddle
cd Paddle
mkdir build
cd build
cmake .. -DWITH_CONTRIB=OFF \
         -DWITH_MKL=OFF  \
         -DWITH_MKLDNN=OFF \
         -DWITH_TESTING=OFF \
         -DCMAKE_BUILD_TYPE=Release \
         -DON_INFER=ON \
         -DWITH_PYTHON=OFF \
         -DWITH_XBYAK=OFF \
         -DWITH_NV_JETSON=OFF \
         -DWITH_GPU=OFF \
         -DWITH_AVX=OFF \
         -DWITH_ARM=ON
make TARGET=ARMV8 -j8
```

### OpenCV 的 Mat 转 vector

参考 https://github.com/PaddlePaddle/PaddleSeg/blob/release/2.6/deploy/cpp/src/test_seg.cc 进行

### [在 QLabel 中显示 OpenCV 图像](https://amin-ahmadi.com/2015/12/16/how-to-display-a-mat-image-in-qt/)

### [QT GUI界面假死的处理](https://blog.csdn.net/wukai_std/article/details/70214854?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EESLANDING%7Edefault-4-70214854-blog-117574159.pc_relevant_landingrelevant&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EESLANDING%7Edefault-4-70214854-blog-117574159.pc_relevant_landingrelevant&utm_relevant_index=5)

### [QString 和 std::string 的互转](https://blog.csdn.net/weixin_43953829/article/details/123322171)

