# 2024-5-1
***version 0.3.0***、
## 新增
- 关于 Line Break
  - token 新增识别
  - lexer 新增识别
  - parser 新增识别
## 优化
- token
  - textToInt, 其中转化由 toInt -> toUInt
- 
## 思路调整
- 之前定下编译两个阶段: 
  - 一阶段: 将指令汇编成二进制编码的 string 形式, 计算空间地址, 预备好符号表(标识符还未替换)
  - 二阶段: 整合二进制编码, 同时将标识符替换掉, 并生成各种文件
- 现在更改为:
  - 一阶段: 将指令汇编成二进制编码的 string 形式, 并存储各个指令的长度 (JMP除外)
  - 二阶段: 计算空间地址, 同时将标识符替换掉, 并整理出符号表, 整合二进制编码, 生成各种文件
- 标识符重定义判断由 parser 转变为 lexer
## 重写
- 将 Identifier, IdentifierList 从 ast -> vm


# 2024-4-30
***version 0.3.0***
## 优化
- 调整存储静态常量存储: QStringList -> QSet<QString>; QMap -> QHash
- 映射表使用 QHash 替代 QMap 提高查找效率
- token
  - tokenType -> type, 同时改变了判断逻辑, 优化性能 
  - 修改 Type: 
    - 新增: SEGMENT, ENDS, END
    - LABEL -> IDENTIFIER
    - DB DW DD DQ DT -> DEFINE
    - 映射关系 一对一放在 QHash, 多对一放在 QSet+QHash
      - 又 舍弃上面方案, 全部用 QHash, 效率更高, 逻辑也更简单, 不过上面方案模块划分更清晰
    - 更改静态存储
      - sm_mnemonics_A, sm_symbols_A 合并入 sm_typeNames
      - sm_mnemonics_B -> sm_mnemonics
      - sm_symbols_B -> sm_singleSymbols; sm_symbols_C -> sm_doubleSymbols


# 2024-4-29
***version 0.3.0***
## 优化
- token
  - 增添了 endColumn
  - 修复了 textToInt 无法识别 非0开头十六进制数的问题
  - 修复了对非数字字符[a-fA-F]的非法判断为非法数字问题问题
  - 修复了整数0识别为浮点数的问题
  - 新增 token: PTR, SHORT, NEAR, FAR, WORD, DWORD
- lexer
  - 修复了 restore 空格计算错误问题
- parser
  - 修复了 program 空 stmt 紧跟换行 会结束分析的问题
## 重写
### ast
- 取消了 goError 和 isError, 通过递归传递 error 感觉收益很低
- Program 添加 haveError
### parser
- 增添了 expectPeekTokenNot
- 优化了 program 的实现
- 优化了 clear (空间占用)
- 完成了对 '#'指令的分析
- 完成了对标签定义语句的分析
### assembler


# 2024-4-20
***version 0.3.0***
## 优化
- 优化了: 构造函数, 拷贝构造, 析构函数等 的写法, 使用 default
## 重写
### 设计
***注意: 编译阶段应该将尽可能多的工作交给 assembler, 但是 parser 应该完成必要且不留重复工作给 assembler***
- ast
  - 取消了 traversal, 将 json 放到'ast/json.cpp' 文件中统一管理
  - 暂时违背开闭等原则, 将频繁访问的变量使用 public 申明, 方便修改, 等node大类几乎设计成熟后再考虑封装
  - 添加了类: Identifier, IdentifierList


# 2024-4-19
***version 0.3.0***
## 删除
- 去掉了 Node 的枚举前缀 'NODE_', 因为: 类内意义不大, 类外 'Node::'
- 取消了 Lexer::setFileName 接口
- 去掉了 Token 的枚举前缀 'TOKEN_', 因为: 类内意义不大, 类外 'Token::'
## 增添
- 新增 Token::Type: ILLEGAL_INTEGER, LINE_BREAK('\\')
- 添加 Lexer::scan 接口, 可以扫描文件 或是 提取好的文本
## 优化
- 优化了 Token 实现
  - MATE_ -> MAKE_X, LOAD_ -> LOAD_X
  - EOF -> TOKEN_EOF, 因为 'EOF' 是 stdio 的宏, 值调整为 -1 和 c 统一
  - 优化了 tokenType 的实现细节
  - 静态的 tokenTypeName -> typeName, 静态的要加前缀 'Token::', 故前缀 'token' 意义不大
- 优化了 Lexer 实现
  - 重构 scan 二次遍历的逻辑, 
  - 优化 scan 的添加错误逻辑, 修复对一个词多次提示
  - 优化 scan 的实现细节
  - 重构了构造以及调用逻辑, 提高复用性和使用的灵活性
- 优化了 ast 实现
  - 静态的 nodeTypeName -> typeName, 静态的要加前缀 'Node::', 故前缀 'node' 意义不大
## 重写
***打算重写 ast, parser, assembler***
### 难点
- 汇编语言和高级语言有些区别, 汇编语言需要在语法分析阶段需要, 解析操作数是否合规, 但是这又涉及到汇编阶段要做的事情, 确定完操作数合规, 几乎等价于完成汇编;
- 如果将这一步操作放在 assembler 中进行, 相当于对抽象语法树又进行了一次语法分析并求值
### 重新定位
- 将 compile 分为两个阶段
  - 一阶段: 将指令汇编成二进制编码的 string 形式, 计算空间地址, 预备好符号表(标识符还未替换)
  - 二阶段: 整合二进制编码, 同时将标识符替换掉, 并生成各种文件
- parser: 进行语法分析同时完成 compile 一阶段
  - 可以存在多个, 对多个文本解析
  - 生成抽象语法树 AST
  - AST 根节点 program 存储符号表 QMap
- assembler: 完成 compile 二阶段, 并与文件交流 (不抛出错误和警告, 所有错误警告由 parser 抛出)
  - 与 parser 不同, assembler 应该仅存在一个, 编译时调用
  - 传入 AST 不是坏树则可以编译
  - 读取 program 中的格式, 生成符号表(文件)
  - 根据符号表生成二进制文件
- *也就是说, 文本检测到 parser 终止, 编译要调用 assembler*


# 截至 2024-4-15
***version 0.2.1***
- ui方面达到运行的最低标准, 但是仍有许多bug和空缺功能
- 底层实现: 能勉强看, 但是很烂
  - parser mov指令, '#'指令等
  - ast 相应的结点
  - assembler 和 vm 在实现阶段发现了定位不清, 职能划分不明确诸多问题, 导致整体思路非常混乱, 先使用 取巧 的方式实现其功能
- 之后需要从头梳理思路, 重写 parser, ast, assembler, vm


# 2024-4-7
- 使用JSON生成抽象语法树(AST)
- 解决优化value后除法取余运算卡死的bug
- 解决数字的前缀后缀识别冲突bug


# 2024-4-6
- 优化了'#'指令的识别以及报错和警告
- 增加了 ast::MultipleStatement 用来构造前缀指令结点
- 实现了 ast::Value类 以便递归下降求值, 支持运算, 但仅需用到前两种:
  - ~x -x +x * / % + - << >> & ^ |
  - < <= > >= == !=
  - ~= *= /= %= += -= <<= >>= &= ^= |= =
- 对字符串增加了转义符的支持以及相关警告和报错


# 2024-4-5
- 完成了'#'指令的识别
- 修改了前缀支持: [-] -> [-+~]
- 增添了对浮点数的识别以及不支持处理的报错
- 将parser对非法单词(illegal)的处理独立出来
- 修改了ast对error结点的遍历逻辑


# 截至 2024-4-4
## 词法分析器
- token 和 lexer 几乎成熟, 目前支持识别:
  - '#'指令: 
    - MAKE_BIN
    - MAKE_COM
    - MAKE_EXE
    - LOAD_SEGMENT=
    - LOAD_OFFSET=
    - REG=
  - 单引号和双引号的字符串
  - 数字:
    - 16进制前缀 0x ...
    - 2 8 10 16 四种进制后缀 B O D H
    - 前缀后缀不可共存
    - 通过二次扫描解决纯字符16进制数需要'0'开头, 但不确定后续是否会存在其他影响
  - 标签
  - 注释;
  - 符号: 
    - 运算: ~ * / % + - << >> & ^ | < <= > >= == != 
    - 其他: = , : ? $ # ; () []
  - 所有(应该)8086汇编指令助记符
  - 部分伪指令助记符
## 语法分析器
- ast 和 parser 正在火速开发, 目前可以识别算术表达式
- 下一步: 
  - 1. 可以识别'#'指令
  - 2. 可以生成mov指令的抽象语法树
  - 3. 可以将mov指令汇编
  - 4. 可以将mov指令反汇编
  - 5. 完成虚拟机可以执行汇编指令
## 图形界面
- 文本编辑器
  - 语法高亮(所有词类已划分好)
  - 报错信息显示
  - 根据信息标记行和列范围
  - 实时监测是否有错误信息(设置定时器)
- 完成虚拟机测试的子窗口
