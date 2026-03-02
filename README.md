# Humans
Blazingly fast utility to convert time and bytes into human readable format ⚡

# Install
```sh
pip install humans-formatter
```

# Usage
```py
import humans

# Basic human_time
print(humans.human_time(3600000))  # "1h"

# Rounded human_time
print(humans.human_time(90000, round=True))  # "2m"

# Compound human_time
print(humans.human_time(90000, compound=True))  # "1m 30s"

# Negative human_time
print(humans.human_time(-90000, compound=True))  # "-1m 30s"

# Basic human_bytes
print(humans.human_bytes(1024*1024*1024))  # "1.00 GiB"

# Negative human_bytes
print(humans.human_bytes(-1536))  # "-1.50 KiB"
```

# Parameters
- human_time(milliseconds: int, round=False, compound=False)
    - milliseconds: Time in milliseconds (can be negative)
    - round: If True, rounds to nearest largest unit
    - compound: If True, shows all units (e.g., "1h 2m 3s")

- human_bytes(size: int)
    - size: Number of bytes (can be negative)
    - Converts to human-readable binary units (B, KiB, MiB, GiB, etc.)