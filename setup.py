from setuptools import setup, Extension

module = Extension("humans", sources=["humans.c"])

# Description
with open(("README.md"), encoding="utf-8") as readmeh:
    readme = readmeh.read()

setup(
    name="humans_formatter",
    version="0.1",
    description="Utility to convert time and bytes to human readable format",
    long_description=readme,
    long_description_content_type="text/markdown",
    ext_modules=[module],
)
