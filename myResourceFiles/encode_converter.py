import os
import sys


def main():
    try:
        filename = sys.argv[1]
        content = open(filename, "r", encoding="UTF-8-SIG").read()
    except Exception as err:
        print(f"Unable to read file with UTF-8 BOM encoding, error={err}")
        os.exit(1)  # Fails CI

    try:
        open(filename, "w", encoding="GBK").write(content)
    except Exception as err:
        print(f"Unable to write file with GBK encoding, error={err}")
        os.exit(1)

    print(f"File {os.path.basename(filename)} has been converted to GBK encoding.")


if __name__ == "__main__":
    main()
