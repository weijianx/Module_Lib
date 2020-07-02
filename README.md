# Module_Lib
module &amp;&amp; lib in common

-----公共模块及库文件

说明：

~~~
要求：

  1）流程图（或方框图），图的格式为BMP,jpg

  2) 源代码（例如.h .c 文件）

  3）代码注释doxygen格式

  4）模块引用的库在注释文档说明

  5）模块或库的文档说明首页中注明你的编译环境（例如系统及系统版本；编译器及版本）

在你的本地工作区的目录结构如下：

XXX_Module （模块目录）
   ----sourece （源代码目录）
       --- xxx.h
       ----xxx.c
       ----README.txt  (在这个文档说明在首页上列出上面的第5点)
    

   ----Lib （由源代码生成的封装起来的库目录）
       ----xxx.lib 
            :
       ----README.txt (说明生成库的输入及输出要求)
       
~~~



在使用模块及库如发现由bug或要求修改的，先在fig_bug的分支中提出issue，讨论之后可以提交PR到fix_bug分支测试