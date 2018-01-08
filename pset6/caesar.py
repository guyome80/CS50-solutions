#include <stdio.h>
import cs50
import sys
import string

k = 0
newphrase = []

# main program
def main():
    # check that the client inputs are correct
    if len(sys.argv) != 2:
        print("Please use only one and positive integer")

    # mod26 to the k integer given by the client

    global k
    k = get_k(int(sys.argv[1]))

    # get the phrase from user
    phrase = cs50.get_string()

    # defining the new phrase as a list of phrase characters
    for i in range(len(phrase)):
        keychar = Encrypt(phrase[i])
        global newphrase
        newphrase.append(keychar)

    # print the new list of character
    for i in range(len(newphrase)):
        sys.stdout.write("{}".format(newphrase[i]))

    #end of printing!
    print("")

# gives the k script with mod26, usefull for the alphabet !
def get_k(n):
    n = n % 26
    return n

# encrypt a character
def Encrypt(a):
    if str.isalpha(a) == True:
        j = ord(a)

        # encrypt a lower character with mod26
        if str.islower(a) == True:
            j = j - 97
            j = (j + k) % 26
            j = j + 97

        # encrypt a uppper character with mod26
        else:
            if str.isupper(a) == True:
                j = j - 65
                j = (j + k) % 26
                j = j + 65

        # convert the ascii number in a character
        a = chr(j)
    return a

if __name__ == "__main__":
    main()
