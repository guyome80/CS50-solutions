# include <stdio.h>
# include <cs50.h>

int main(void)
{
    int Min;
    printf("how many minutes are spent in the shower ?\n");
    Min = GetInt();
    printf("Minutes: %d\n", Min);
    int Btlls = 0;
    Btlls = Min * 192 / 16;
    printf("Bottles: %d\n", Btlls);
}