# very professional testing script
# please clap

from timeit import timeit


results = """
- Is humans faster: {}
- Increased speed by {} times
- Time took:
    humans: {}
    origin: {}
"""


def compare():
    time_cshared = timeit(
        "humans.human_time(100*60*60)", setup="import humans", number=10000
    )
    time_origin = timeit(
        "origin.TimeFormatter(100*60*60)", setup="import origin", number=10000
    )

    humanbytes_cshared = timeit(
        "humans.human_bytes(1024*1024*1024)", setup="import humans", number=10000
    )
    humanbytes_origin = timeit(
        "origin.humanbytes(1024*1024*1024)", setup="import origin", number=10000
    )

    print(
        results.format(
            time_cshared < time_origin,
            f"{time_origin / time_cshared:.2f}",
            time_cshared,
            time_origin,
        )
    )

    print(
        results.format(
            humanbytes_cshared < humanbytes_origin,
            f"{humanbytes_origin / humanbytes_cshared:.2f}",
            humanbytes_cshared,
            humanbytes_origin,
        )
    )


compare()