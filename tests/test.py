from timeit import repeat

NUMBER = 1_000_000
REPEAT = 5

def bench(stmt, setup):
    times = repeat(stmt, setup=setup, number=NUMBER, repeat=REPEAT)
    return min(times)

def compare():
    t1 = bench(
        "f(360000)",
        "import humans; f = humans.human_time",
    )
    t2 = bench(
        "f(360000)",
        "import origin; f = origin.TimeFormatter",
    )

    b1 = bench(
        "f(1024*1024*1024)",
        "import humans; f = humans.human_bytes",
    )
    b2 = bench(
        "f(1024*1024*1024)",
        "import origin; f = origin.humanbytes",
    )

    print("Time Formatter")
    print(" Faster:", t1 < t2)
    print(" Speedup:", f"{t2/t1:.2f}x")
    print(" humans:", t1)
    print(" origin:", t2)
    print()

    print("Human Bytes")
    print(" Faster:", b1 < b2)
    print(" Speedup:", f"{b2/b1:.2f}x")
    print(" humans:", b1)
    print(" origin:", b2)

compare()