# Maintenance registry reader

One-shot, read-only PS5 payload using `sceRegMgrGetInt` and `sceRegMgrGetStr`.
It reads an explicit allowlist of 29 system, power and network fields, followed
by three DNS fields from the currently selected network profile (1–10).
Key IDs and types come from
[upstream regdump](https://github.com/ps5-payload-dev/regdump/blob/master/regmgr.h).
No credentials, tokens, account IDs, Wi-Fi passwords or SSIDs are queried.

Build with `PS5_PAYLOAD_SDK=/path/to/sdk make`, load the ELF once using Payload
Manager, then retrieve `/data/maintenance-status.txt` through FTP or shsrv.
Wait for the payload to finish before reading the output. The output may contain
network configuration: keep it private. No listener or autoload entry is added.
The only write is this diagnostic report; registry settings are never changed.

## Verified on 12.20 — 2026-11-07

- 32 queried fields: 29 successful, 3 returned `0x800d0203`.
- Automatic firmware download, install and update flags: all zero.
- Language field: 11; the user previously confirmed the Chinese UI.
- Active profile DNS fields read successfully and match the user's configured
  third-party resolver; secondary DNS is empty. This resolves the prior
  ambiguity from the Tailscale interface's router DNS display.
- Generic LAN/Wi-Fi DNS fields were empty; they must not be treated as the
  current profile. The reader selects the profile through `NET_COMMON_current_profile`.
- Unsupported fields: USB charge minutes, generic IPv6 DNS flag and address.
- Power flags are reported as raw integers: enumerated values have not been
  independently mapped to all UI labels, so do not interpret them as booleans.

Additional read-only checks through the existing shsrv and Payload Manager:

- kstuff, ShadowMount, FTP and shell processes present; a game process present.
- User storage reports roughly 615 GiB available; update directory empty.
- `hw.ncpu` returns 16 logical CPUs; `kern.boottime` is readable.
- `hw.physmem` and `dev.cpu.0.temperature` are unavailable through sysctl.
  No reliable temperature or physical-memory total is claimed.

No DNS change, updater invocation, extra debugger, persistent daemon or Linux
loader was used. SDK/compiler/binaries and raw device reports remain untracked.
