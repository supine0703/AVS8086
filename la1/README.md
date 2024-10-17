
我通过下面指令停止了对 `.vscode/settings.json` 的更新
```sh
git update-index --assume-unchanged .vscode/settings.json
```

可以通过下面指令恢复
```sh
git update-index --no-assume-unchanged .vscode/settings.json
```
