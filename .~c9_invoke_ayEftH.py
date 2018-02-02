from cs50 import get_int

def main():
    while True:
        # prompt user for positive integer for height
        height = get_int("Height: ")
        if height > 0 and height < 23:
            break

    # access every row
    for i in range(height):
        # print spaces
        for j in range(height - i - 1):
            print(" ", end="")
            j += 1
        # print hashes
        for k in range(i + 2):
            print("#", end="")
            k += 1
        # add new row
        i += 1
        # add new line
        print()
    return 0


if __name__=="__main__":
    main()