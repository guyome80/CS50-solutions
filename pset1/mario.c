# include <stdio.h>
# include <cs50.h>

int main(void)
{
    // ask for the height of the pyramide wanted
    int High;
    do
    {
        High = GetInt();
        printf("Height :\n");
        //High = GetInt();
    }
    while ((High <= 1) || (High >= 24));

    //print the pyramide
    //printf("Height: %d\n", High);
    int Space;
    int Brick;
    int Line;
    for (Line = 1; Line < High + 1; Line++)
    {
        //print the spaces
        
        {
            for (Space = High - 1; Space >= Line; Space--)
            {
                //print the bricks of the pyramide
                printf(" ");
            }
            for (Brick = 0; Brick <= Line; Brick++)
                {
                   printf("#");
                }
        }
        printf("\n");
    }
}
    //printf("Minutes: %d\n", Min);
    //int Btlls = 0;
    //Btlls = Min * 192 / 16;
    //printf("Bottles: %d\n", Btlls);
    //}