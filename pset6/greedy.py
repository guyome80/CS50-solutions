import cs50
# minimizes the number of coins for any amount of change
# gives the yield detail for each coins


# get user change value
change = -1
while (change < 0):
    print("How much change the customer owes ?")
    change = cs50.get_float()

# tranform change into cents integer value rounded
x = change * 100.00
iChange = round(x)

# quarter used
quarterNum = 0
while iChange >=  25:
    iChange = iChange - 25
    quarterNum = quarterNum + 1

# dime used
dimeNum = 0
while iChange >=  10:
    iChange = iChange - 10
    dimeNum = dimeNum + 1

# nickel used
nickelNum = 0;
while iChange >=  5:
    iChange = iChange - 5
    nickelNum = nickelNum + 1

# penny used
pennyNum = 0
while iChange > 0:
    iChange = iChange - 1
    pennyNum = pennyNum + 1

# prints number of coin owed into the change
total = quarterNum + dimeNum + nickelNum + pennyNum
print(total)