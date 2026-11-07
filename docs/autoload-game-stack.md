# 游戏与远程维护自动加载

2026-11-07，用户确认启用以下组合，并特别要求包含游戏加载器。

Payload Manager 自动加载已开启，保留原延迟 5 秒，列表顺序如下：

1. shsrv-ps5_v0.20.elf
2. tailscale.elf
3. kstuff-lite_v1.11.elf
4. ShadowMountPlus_1.7beta11.elf
5. ftpsrv_v0.21.1.elf

kstuff 排在 ShadowMountPlus 之前。ShadowMountPlus 是目录游戏加载器；此配置不会自动启动某一款游戏。

已通过 get_config 读回并逐项校验；autoload_status 也返回新列表。修改前确认这些服务已运行，因此未重复注入。未重启主机，尚未验证重启后的完整自动加载过程。自动加载在漏洞环境与 Payload Manager 启动后生效，不代表未进入漏洞环境也会运行。

其他配置保持不变。nanoDNS、其他 HEN、Linux Loader、安装器及一次性诊断工具均未加入自动加载。原配置保存在本机忽略目录 backups 中。
