from distutils.core import setup, Extension

def main():
    setup(
        name="fputs",
        version="1.0.0",
        description="Python interface for the fputs C library function",
        author="Siddhartha Dhar Choudhury",
        author_email="sdharchou@gmail.com",
        ext_modules=[Extension("fputs", ["fputsmodule.c"])]
    )

if __name__ == "__main__":
    main()