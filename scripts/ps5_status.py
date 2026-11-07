#!/usr/bin/env python3
"""Read Payload Manager status without exposing Tailscale account information."""
import argparse
import json
import urllib.request


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--host', required=True, help='PS5 LAN hostname or IP')
    args = parser.parse_args()
    base = f'http://{args.host}:8084'
    for endpoint in ('processes_list', 'get_config'):
        with urllib.request.urlopen(f'{base}/{endpoint}', timeout=10) as response:
            data = json.load(response)
        if endpoint == 'processes_list':
            data = [p for p in data['processes'] if p.get('is_daemon') or p['name'] == 'eboot.bin']
        print(endpoint)
        print(json.dumps(data, indent=2, ensure_ascii=False))


if __name__ == '__main__':
    main()
