# PS5 Toolkit

本项目整理一次 PS5 12.20 环境中的远程维护、目录传输与排障经验。
只包含脚本与说明，不包含游戏、固件、密钥、账号、设备地址或第三方 payload 二进制文件。

## 环境

- 电脑：Python 3，仅使用标准库。
- PS5：已运行 Payload Manager；上传需要 ftpsrv，目录应用需要适配的加载环境。
- 本次使用过 Payload Manager 0.5.2、shsrv 0.20、ftpsrv 0.21.1、kstuff-lite 1.11、ShadowMountPlus 1.7beta11、Tailscale 0.7.3。
- 这些是本次使用版本，不代表最新版本，也不保证支持其他固件。

## 使用

将下面的 `PS5_HOST` 替换为设备的实际地址。脚本不保存设备地址。

```bash
python3 scripts/ps5_status.py --host PS5_HOST
python3 scripts/upload_folder.py --host PS5_HOST /path/to/homebrew
```

上传目录必须包含 `sce_sys/param.json`，其中 `titleId` 为九位大写字母或数字。
上传先写入独立暂存目录，对每个文件读回并验证 SHA-256，再移动到 `/data/homebrew/TITLEID`。
已有同名安装会拒绝覆盖；失败暂存目录保留供检查，不会自动发布或启动应用。
请勿同时运行针对同一 titleId 的多个上传任务。

## 排障

参见 [操作记录](docs/operations.md)。

## 验证范围

原始操作曾完成目录传输、主程序校验以及应用进程启动。这里整理后的通用脚本通过 Python 编译和命令行帮助检查，尚未重新进行完整设备端上传测试。

## 上游项目

- [shsrv](https://github.com/ps5-payload-dev/shsrv)
- [ShadowMountPlus](https://github.com/drakmor/ShadowMountPlus)
- [Tailscale for PS5](https://github.com/holdmysocks/ps5-tailscale)
- [PS5 payload SDK](https://github.com/ps5-payload-dev/sdk)
- [PS5 UltraPack](https://github.com/knutwurst/ps5-ultrapack)

各上游项目遵循各自许可证。本仓库不复制其签名器源码或二进制文件。
