# Humans
Blazingly fast utility to convert time and bytes into human readable format ⚡

# Install
```sh
pip3 install humans-formatter
```

# Usage
```py
import humans


print(humans.human_time(100*60)) # 6s

print(humans.human_bytes(1024*1024*1024)) # 1024.00 MiB
```