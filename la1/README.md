
我通过下面指令停止了对 `.vscode/settings.json` 的更新
```sh
git update-index --assume-unchanged .vscode/settings.json
```

可以通过下面指令恢复
```sh
git update-index --no-assume-unchanged .vscode/settings.json
```

### 编程约定

#### 格式化

使用当前文件夹下面的 `.clang-format` 文件的规则最好直接使用 `clang-format` 进行格式化

#### 关于 this 指针的使用

- 在类函数中，其直接成员**不加** `this` 
- 在类函数中，其非直接成员 (父类成员) **要加** `this`
- 在拷贝、移动这类语句中，作为区分 **要加** `this`

#### 关于命名

- 静态变量加前缀 `s_`
- 成员变量加前缀 `m_`
- 全局变量加前缀 `g_`
- 静态成员变量加前缀 `ms_`
- 可直接公有访问的成员变量**不加前缀**
- 类命名采用**大驼峰命名**
- 函数名采用**小驼峰命名**
- 变量命名采用**小写下划线命名**
- 宏定义采用**大写下划线命名**
- 字母和数字之间应该加`_`
- Qt UI 中的组件，采用 `类型_名字` 的方式命名，例如：`btn_choose`
- Qt 自动槽函数 按照 Qt 标准就好

