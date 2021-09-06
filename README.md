# screen-space-reflection
ssr demo
利用g-buffer实现一个简易的线追踪demo。

效果如图：
![image](https://github.com/newMember1/screen-space-reflection/blob/master/pics/Screenshot%20from%202021-09-06%2023-22-43.png)

使用方法(linux)：
```shell
mkdir build
cd build
cmake ..
make
./screen-space-reflection
```

todo:
1.  添加带贴图模型来验证binary search是否能有效改善效果。
2.  边缘部分在采样step过大时会被跳过，造成更明显的锯齿，考虑对其卷积来实现平滑。

reference:
1.  https://www.cnblogs.com/yangrouchuan/p/7574405.html
2.  https://zhuanlan.zhihu.com/p/49459651
