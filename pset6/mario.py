import cs50

height = -1
while height < 0 or height > 23:
    print("Height ?")
    height = cs50.get_int()

# prompt a pyramide
i = 0

while i < height:
    # print each line
    for j in range(height - i - 1):
        print(" ", end='')
    for k in range(i + 2):
        print("#", end='')
    print("")
    i = i + 1