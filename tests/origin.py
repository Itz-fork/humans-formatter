def TimeFormatter(milliseconds: int) -> str:
    seconds, milliseconds = divmod(int(milliseconds), 1000)
    minutes, seconds = divmod(seconds, 60)
    hours, minutes = divmod(minutes, 60)
    days, hours = divmod(hours, 24)

    if minutes > 0:
        return f"{minutes}m"
    elif seconds > 0:
        return f"{seconds}s"
    elif hours > 0:
        return f"{hours}h"
    elif days > 0:
        return f"{days}d"
    else:
        return f"{milliseconds}ms"


def humanbytes(size):
    if not size:
        return ""
    power = 2**10
    n = 0
    pwrN = {0: " ", 1: "Ki", 2: "Mi", 3: "Gi", 4: "Ti"}
    while size > power:
        size /= power
        n += 1
    return str(round(size, 2)) + " " + pwrN[n] + "B"
