
> d_font: default used font


### 使用字体说明

- 主字体: JetBrains Mono v2.304 (en)  <[官方链接](https://www.jetbrains.com/lp/mono/)>
- 副字体: 优设鲨鱼菲特健康体 (zh)  <[官方链接](https://www.uisdc.com/uisdc-sharkfit-font)>

以上字体均可免费商用

> Qt 的 Edit 默认行高是根据第一个字体的行高来的，并且不可设置；如果后续字体行高更高，则会改变行高，且影响行号标记。
> 因此，我制作了一个仅在 `unicode: 0x1fdc` (未定义) 的位置含有一个空格的字体，设了 200 的行高，
> 以抵消 Qt 的 Edit 因字体行高不一致所带来的不良影响，如果仍有影响，可以将行高继续拉大。
