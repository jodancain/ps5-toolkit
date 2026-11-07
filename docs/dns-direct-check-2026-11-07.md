# DNS 直连复测

2026-11-07。先前 Fake-IP 结果受到维护电脑 Mihomo TUN 的 DNS 路由影响。指定物理网卡的源地址后，路由检查确认走普通网关，不再经过 Mihomo。未修改系统路由、代理或 PS5 设置。

对用户提供的第三方 DNS，测试七个域名、A/AAAA 两种记录，每种三次，共 42 次 UDP 查询，超时为三秒、无重试。41 次收到有效响应，1 次 example.com A 查询超时；成功请求耗时 363–433 ms。此小样本不能代表长期在线率，超时也不能单独归因于服务器。

- example.com 与 github.com：正常解析（GitHub AAAA 无记录）。
- djp01.ps5.update.playstation.net、fus01.ps5.update.playstation.net、fjp01.ps5.update.playstation.net：18 次查询全部返回 A=0.0.0.0 或 AAAA=::，这些已测更新域名被屏蔽。
- auth.api.sonyentertainmentnetwork.com 与 store.playstation.com：A 查询仍返回公网 CDN 地址，未在 DNS 层全面屏蔽 Sony 登录和商店。

只发送 DNS 查询，未访问 Sony 服务、登录 PSN 或触发更新。测试来自维护电脑，并非 PS5 原生解析链路；备用 DNS、IPv6 DNS 和应用自己的解析路径不在本次验证范围。

结论：该 DNS 当前可用，已测更新域名的屏蔽有效，但并非所有 Sony 服务都被阻止；不能保证长期稳定或完整防更新。
