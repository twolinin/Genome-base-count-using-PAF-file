#include<stdio.h>

int main()
{
    char in;

    int insertion_num  = 0;
    int insertion_base = 0;

    int deletion_num   = 0;
    int deletion_base  = 0;

    while(scanf("%c",&in)!=EOF)
    {
        int cat = 0;
        int tmp_in = 0;
        int tmp_dn = 0;

        //find cs:Z:=
        while(cat<5)
        {
            scanf("%c",&in);
            if( in == 'c' || in == 's' || in == ':' || in == 'Z' )
                cat++;
            else
                cat = 0;
        }

        scanf("%c",&in);

        while( in != '\n' )
        {
            //printf("%c",in);
            if( in == '+' )
            {
                insertion_num++;
                while(scanf("%c",&in)!=EOF)
                {
                    if( in == '+' || in == '-' || in == '=' || in == '*' || in == '\n' )
                        break;
                    insertion_base++;
                    tmp_in++;
                }
            }
            if( in == '-' )
            {
                deletion_num++;
                while(scanf("%c",&in)!=EOF)
                {
                    if( in == '+' || in == '-' || in == '=' || in == '*' || in == '\n' )
                        break;
                    deletion_base++;
                    tmp_dn++;
                }
            }
            if( in == '=' || in == '*' )
            {
                while(scanf("%c",&in)!=EOF)
                {
                    if( in == '+' || in == '-' || in == '=' || in == '*' || in == '\n' )
                        break;
                }
            }
        }

        printf("%d %d\n",tmp_in,tmp_dn);

    }

        printf("num of insertion     : %d\n",insertion_num);
        printf("total insertion base : %d\n",insertion_base);
        printf("num of deletion      : %d\n",deletion_num);
        printf("total deletion base  : %d\n",deletion_base);

    return 0;
}
