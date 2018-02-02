from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # split both strings into lines
    filea = a.splitlines()
    fileb = b.splitlines()

    # compare both lines of strings and see which lines are same
    together = list(set(filea) & set(fileb))
    print(together)
    return together


def sentences(a, b):
    """Return sentences in both a and b"""

    # split both strings into sentences
    lista = sent_tokenize(a, language='english')
    listb = sent_tokenize(b, language='english')

    #compare both sentences of strings and see which ones are the same
    same = list(set(lista) & set(listb))
    print(same)
    return same

def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    def helper(x):
        # split string to create list of words within string
        split = x.split(" ")
        # iterate over each word in the list
        done = set()
        for word in split:
            # iterate over each character within word
               for i in range(len(word)):
                    if i+n > len(word):
                        break
                    done.add(word[i:i+n])
        return done

    list1 = helper(a)
    list2 = helper(b)

    similar = list(set(list1) & set(list2))
    return similar
