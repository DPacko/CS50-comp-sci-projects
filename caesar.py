from cs50 import get_string
import sys


def main():
    # get correct # of c-l arguments
    if len(sys.argv) != 2:
        print("not desired count of arguments")
        exit(1)

    # convert key from string to integer
    k = int(sys.argv[1])
    if k < 0:
        print("can't be a negative number!")
        exit(1)

    # get plaintext from user
    p = get_string("plaintext: ")
    print("ciphertext: ", end="")

    # print out ciphertext
    for c in p:
        if c.isalpha():
            if c.isupper():
                n = ((((ord(c) + k) - 65) % 26) + 65)
                print(f"{chr(n)}", end="")
            else:
                n = ((((ord(c) + k) - 97) % 26) + 97)
                print(f"{chr(n)}", end="")
        else:
            print(f"{c}", end="")

    print()
    exit(0)


if __name__ == "__main__":
    main()
