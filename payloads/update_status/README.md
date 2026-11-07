# Read-only update settings

Reads three integer registry values through `sceRegMgrGetInt`; no setters,
network calls, update checks, or account reads are used. The only write is a
small report at `/data/update-status.txt`.

Registry IDs are documented in upstream
[regdump/regmgr.h](https://github.com/ps5-payload-dev/regdump/blob/master/regmgr.h).

Build with the official PS5 payload SDK and its LLVM dependencies:

```sh
export PS5_PAYLOAD_SDK=/path/to/ps5-payload-sdk
make
```

Load `update-status.elf` once with Payload Manager, then read the report through
shsrv or FTP. No autoload entry is needed. A nonzero return code or unexpected
value must not be interpreted as a disabled setting.

## Device verification — 2026-11-07

On the existing 12.20 console, all reads succeeded:

| Registry value | Key | Return code | Value |
| --- | --- | --- | --- |
| auto_download | 0x02800500 | 0 | 0 |
| auto_install | 0x02800e00 | 0 | 0 |
| auto_update | 0x02800800 | 0 | 0 |

The automatic download and automatic installation flags are both disabled.
This supersedes earlier operation notes that their state was unconfirmed.
No setting was changed, DNS was retained, and no firmware update was initiated.
These flags do not prevent a user from explicitly requesting a manual update.

Built with SDK v0.45 and LLVM 21. The checked-in files are the small reader's
source, build recipe and this report; SDK, compiler and ELF are not committed.
