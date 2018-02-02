from cs50 import get_float


def main():
    while True:

        # set variable count
        count = 0

        # prompt user for positive integer
        change = get_float("How much change is owed?: ")
        if change > 0:
            break

    # convert change to cents
    change = round(change * 100)


    add = change // 25
    count += add
    change %= 25

    add = change // 10
    count += add
    change %= 10

    add = change // 5
    count += add
    change %= 5

    add = change // 1
    count += add
    change %= 1

    print("You get back {} coins".format(count))
    return 0


if __name__ == "__main__":
    main()