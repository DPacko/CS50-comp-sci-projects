from cs50 import get_string
import sys


def main():
    # get correct # of c-l arguments
    if len(sys.argv) < 2:
        print("write a key")
        exit(1)

    # add variable to identify Key
    if sys.argv[1].isalpha():
        k = (sys.argv[1])
    else:
        print("only letters")
        exit(1)

    # get user's plaintext
    p = get_string("plaintext: ")
    print("ciphertext: ", end="")

    # create variable for key's index
    keyspot = 0
    # set key's letters to all lowercase and change it depending on if user's
    # plaintext is lower or uppercase
    k = k.lower()

    # go in plaintext and cipher it, letter by letter, using the key
    for i in p:
        # make sure letter is alphabetical
        if i.isalpha():
            # execute this line if letter is uppercase
            if i.isupper() == True:
                # change plaintext letter and key letter to ascii, subtract
                # them by ascii value, determine where they are in alphabetical
                # number, then add back the ascii value
                n = ((ord(i) - 65) + ord(k[keyspot]) - 97) % 26 + 65
                # print the ciphered plaintext
                print(f"{chr(n)}", end="")
                # go to next letter in key
                keyspot = (keyspot + 1) % len(k)
            # execute this line if letter is lowercase
            else:
                n = ((ord(i) - 97) + ord(k[keyspot]) - 97) % 26 + 97
                print(f"{chr(n)}", end="")
                keyspot = (keyspot + 1) % len(k)
        else:
            print(f"{i}", end="")

    print()
    exit(0)


if __name__ == "__main__":
    main()