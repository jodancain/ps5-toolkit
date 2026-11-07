#!/usr/bin/env python3
"""Stage a homebrew folder via FTP, verify all hashes, then publish by rename."""
import argparse
import ftplib
import hashlib
import json
import re
import uuid
from pathlib import Path


def ensure_dir(ftp, path):
    try:
        ftp.mkd(path)
    except ftplib.error_perm:
        # Do not mistake a permission failure for an existing directory.
        old = ftp.pwd()
        ftp.cwd(path)
        ftp.cwd(old)


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as src:
        for block in iter(lambda: src.read(262144), b''):
            h.update(block)
    return h.digest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--host', required=True)
    parser.add_argument('--port', type=int, default=2121)
    parser.add_argument('folder', type=Path)
    args = parser.parse_args()
    root = args.folder.resolve(strict=True)
    title = json.loads((root / 'sce_sys/param.json').read_text())['titleId']
    if not re.fullmatch(r'[A-Z0-9]{9}', title):
        raise ValueError('Unexpected titleId')
    paths = sorted(root.rglob('*'))
    if any(p.is_symlink() or '\n' in str(p) or '\r' in str(p) for p in paths):
        raise ValueError('Symlinks and newline filenames are not supported')
    files = [p for p in paths if p.is_file()]
    target = '/data/homebrew/' + title
    staging = '/data/ps5-staging/' + title + '-' + uuid.uuid4().hex[:12]
    with ftplib.FTP() as ftp:
        ftp.connect(args.host, args.port, timeout=30)
        ftp.login()
        # ftpsrv defaults to decoding SELF on retrieval. Verification needs raw bytes.
        reply = ftp.sendcmd('SELF')
        if 'disabled' not in reply.lower():
            raise RuntimeError('Expected raw SELF transfer mode: ' + reply)
        ensure_dir(ftp, '/data/homebrew')
        entries = []
        ftp.retrlines('LIST /data/homebrew', entries.append)
        if any(line.split()[-1] == title for line in entries if line.split()):
            raise FileExistsError(target)
        ensure_dir(ftp, '/data/ps5-staging')
        ftp.mkd(staging)
        for path in files:
            relative = path.relative_to(root)
            for parent in reversed(relative.parents):
                if str(parent) != '.':
                    ensure_dir(ftp, staging + '/' + parent.as_posix())
            remote = staging + '/' + relative.as_posix()
            expected = digest(path)
            with path.open('rb') as src:
                ftp.storbinary('STOR ' + remote, src, 262144)
            actual = hashlib.sha256()
            ftp.retrbinary('RETR ' + remote, actual.update, 262144)
            if expected != actual.digest():
                raise RuntimeError('Hash mismatch: ' + str(relative))
            print('Verified:', relative, flush=True)
        # Recheck before publishing. No automatic overwrite of an installed title.
        entries = []
        ftp.retrlines('LIST /data/homebrew', entries.append)
        if any(line.split()[-1] == title for line in entries if line.split()):
            raise FileExistsError(target)
        ftp.rename(staging, target)
        print('Published:', target)


if __name__ == '__main__':
    main()
