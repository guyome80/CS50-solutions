# include <stdio.h>
# include <cs50.h>

int main(void)
{
    // ask for the height of the pyramide wanted
    float Owed;
    int OwedCents;
    do
    {
        
        printf("O hai! How much change is owed?\n");
        Owed = GetFloat();
        //High = GetInt();
        //printf("%f\n", Owed);
    }
    while (Owed <= 0);
    
    //convert User Money into cents
    OwedCents = Owed * 100.00;
    //printf("%i\n", OwedCents);
    
    
    //Global change datas
    int MoneyNbr = 0;
    int Quarters = 25;
    int Dimes = 10;
    int Nickel = 5;
    int Pennies = 1;
    int QuarterNbr = 0;
    int DimesNbr = 0;
    int NickelNbr = 0;
    int PenniesNbr = 0;
    
    //check of numbers of Quarters
    int temp = OwedCents;
    
    while (OwedCents - ((QuarterNbr * Quarters)) > 0)
    {
        QuarterNbr++;
    }
    
    temp = OwedCents - ((QuarterNbr * Quarters));
    
    if (temp < 0)
    {
        temp = temp + Quarters;
        QuarterNbr--;
    }    
    
    //printf("%d\n", temp);
    //printf("%d\n", QuarterNbr);
    
    //check of numbers of Dimes
    
    while (temp - ((DimesNbr * Dimes)) > 0)
    {
        DimesNbr++;
    }
    
    temp = temp - ((DimesNbr * Dimes));
    
    if (temp < 0)
    {
        temp = temp + Dimes;
        DimesNbr--;
    }    
    
    //printf("%d\n", temp);
    //printf("%d\n", DimesNbr);
    //check of numbers of Nickel
    
    while (temp - ((NickelNbr * Nickel)) > 0)
    {
        NickelNbr++;
    }
    
    temp = temp - ((NickelNbr * Nickel));
    
    if (temp < 0)
    {
        temp = temp + Nickel;
        NickelNbr--;
    }    
    //printf("%d\n", temp);
    //printf("%d\n", NickelNbr);
    //check of numbers of Pennies
    
    while (temp - ((PenniesNbr * Pennies)) > 0)
    {
        PenniesNbr++;
    }
    
    temp = temp - ((PenniesNbr * PenniesNbr));
    
    if (temp < 0)
    {
        temp = temp + Pennies;
        PenniesNbr--;
    }    
    //printf("%d\n", temp);
    //printf("%d\n", PenniesNbr);   
    //add each
    MoneyNbr = QuarterNbr + DimesNbr + NickelNbr + PenniesNbr;
    printf("%d\n", MoneyNbr);
}
    //printf("Minutes: %d\n", Min);
    //int Btlls = 0;
    //Btlls = Min * 192 / 16;
    //printf("Bottles: %d\n", Btlls);
    //}