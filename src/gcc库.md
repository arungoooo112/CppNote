# Linux基础——gcc编译静态库与动态库

>[Linux基础——gcc编译、静态库与动态库（共享库）](https://blog.csdn.net/daidaihema/article/details/80902012)

## gcc编译器

### 1、gcc工作流程

![gcc工作流程](../pic/gcc工作流程.png)

### 2、gcc常用参数

|参数|	用途|
|:--:|:-:|
|-v	|查看版本|
|-o	|产生目标输出文件|
|-I+目录	|指定头文件目录|
|-D	|编译时定义宏|
|-O	|没有优化1/缺省值2/优化级别最高3|
|-Wall	|提示更多警告信息|
|-c	|只编译子程序|
|-E	|生成预处理文件|
|-g	|包含调试信息|


## 静态库

### 1、静态库的命名格式

lib + 库的名字 + .a
例：libMyTest.a (MyTest为静态库的名字）

### 2、静态库作用分析
在项目开发过程中，经常出现优秀代码重用现象，又或者提供给第三方功能模块却又不想让其看到源代码，这些时候，通常的做法是将代码封装成库或者框架，生成的静态库要和头文件同时发布。

优点：

寻址方便，速度快
库在链接时被打包到可执行文件中，直接发布可执行程序即可以使用

缺点：

静态库的代码被加载到可执行程序中，因此体积过大
如果静态库的函数发生改变，必须重新编译可执行程序

### 3、静态库的制作与使用

测试代码的目录结构如下：

~~~
test
|-- include
|-- |-- head.h
|-- lib
|-- main.c
|-- src
    |-- add.c
    |-- div.c
    |-- mul.c
    |-- sub.c 
~~~

include中存放的是头文件，lib中存放的是静态（动态）库，src中存放的是源代码，main.c是发布代码。

#### 第一步：得到*.o文件

    gcc *.c -c -I../include
    
~~~
test
|-- include
|-- |-- head.h
|-- lib
|-- main.c
|-- src
    |-- add.c
    |-- add.o
    |-- div.c
    |-- div.o
    |-- mul.c
    |-- mul.o
    |-- sub.c 
    |-- sub.o
~~~

#### 第二步：创建静态库

    ar rcs libMyTest.a *.o       

将所有.o文件打包为静态库，r将文件插入静态库中，c创建静态库，不管库是否存在，s写入一个目标文件索引到库中，或者更新一个存在的目标文件索引。

接下来，将静态库文件移到lib目录下

    mv libMyTest.a ../lib  

~~~
test
|-- include
|-- |-- head.h
|-- lib
    |-- libMyTest.a
|-- main.c
|-- src
    |-- add.c
    |-- add.o
    |-- div.c
    |-- div.o
    |-- mul.c
    |-- mul.o
    |-- sub.c 
    |-- sub.o
~~~

 并查看库中包含的函数等信息

    nm libMyTest.a               


#### 第三步：使用静态库

第一种方法：gcc + 源文件 + -L 静态库路径 + -l静态库名 + -I头文件目录 + -o 可执行文件名

    gcc main.c -L lib -l MyTest -I include -o app

第二种方法：gcc + 源文件 + -I头文件 + libxxx.a + -o 可执行文件名

    gcc main.c -I include lib/libMyTest.a -o app

## 动态库（共享库）

### 1、动态库的命名格式

lib + 库的名字 + .so
例：libMyTest.so (MyTest为动态库的名字）

### 2、动态库作用分析

共享库的代码是在可执行程序运行时才载入内存的，在编译过程中仅简单的引用，因此代码体积较小。

优点：

节省内存
易于更新，不用重新编译可执行程序，运行时自动加载

缺点：

延时绑定，速度略慢

### 3、动态库的制作与使用

测试代码的目录结构与静态库相同。

#### 第一步：生成与位置无关的.o文件

    gcc -fPIC *.c -I ../include -c   参数-fPIC表示生成与位置无关代码

~~~
test
|-- include
|-- |-- head.h
|-- lib
|-- main.c
|-- src
    |-- add.c
    |-- add.o
    |-- div.c
    |-- div.o
    |-- mul.c
    |-- mul.o
    |-- sub.c 
    |-- sub.o
~~~

#### 第二步：创建动态库

    gcc -shared -o libMyTest.so *.o  
    mv libMyTest.so ../lib

参数：-shared 制作动态库 -o：重命名生成的新文件

~~~
test
|-- include
|-- |-- head.h
|-- lib
    |-- libMyTest.so
|-- main.c
|-- src
    |-- add.c
    |-- add.o
    |-- div.c
    |-- div.o
    |-- mul.c
    |-- mul.o
    |-- sub.c 
    |-- sub.o
~~~

#### 第三步：使用动态库

第一种方法：gcc + 源文件 + -L 动态库路径 + -l动态库名 + -I头文件目录 + -o 可执行文件名

    gcc main.c -L lib -l MyTest -I include -o app

执行失败，找不到链接库，没有给动态链接器（ld-linux.so.2）指定好动态库 libmytest.so 的路径

第二种方法：gcc + 源文件 + -I头文件 + libxxx.so + -o 可执行文件名

    gcc main.c -I include lib/libMyTest.so -o app

执行成功，已经指明了动态库的路径

**如何解决第一种方法中找不到链接库的问题？**

使用命令ldd app可以查看当前的链接库情况

第一种方法：
export LD_LIBRARY_PATH=自定义动态库的路径（只能起到临时作用，关闭终端后失效）
LD_LIBRARY_PATH ： 指定查找共享库（动态链接库）时除了默认路径之外的其他路径，该路径在默认路径之前查找

第二种方法：
将上述命令写入home目录下的.bashrc文件中，保存后重启终端生效（永久）

第三种方法：
直接将动态库拷贝到user/lib的系统目录下（强烈不推荐！！）

第四种方法：
将libmytest.so所在绝对路径追加入到/etc/ld.so.conf文件，使用sudo ldconfig -v 更新