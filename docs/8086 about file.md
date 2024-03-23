# 8086-bin
 - .asm文件: 编程源文件
 - .bin.~asm文件: 临时/备份文件? 比.asm多了注释输出bin的路径
 - .bin.debug文件: 应该是调试文件, 看不懂内容
 - .bin.list文件: 汇编程序列表。机器码 <- 源代码 
   - TIME
   - VERSION
   - [LINE] | LOC: MACHINE CODE | SOURCE
   - 按照.asm逐行对照
 - .bin.symbol文件: 
   - TIME
   - VERSION
   - Name Offset Size Type Segment 
 - .binf文件: 段地址, 偏移地址以及各个寄存器的初始值

# 8086-exe
 - .asm文件: 编程源文件
 - .exe.~asm文件: 临时/备份文件? 比.asm多了注释输出bin的路径
 - .exe.debug文件: 应该是调试文件, 看不懂内容
 - .exe.list文件: 汇编程序列表。机器码 <- 源代码 
   - TIME
   - VERSION
   - [LINE] | LOC: MACHINE CODE | SOURCE
   - 按照.asm逐行对照
 - .exe.symbol文件: 
   - TIME
   - VERSION
   - Name Offset Size Type Segment 
 - EXE HEADER - bytes from 0000 to 01FF inclusive.

# exe-header
 - 01: 4D
 - 02: 5A
 - 03: 