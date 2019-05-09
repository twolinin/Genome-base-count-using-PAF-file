#include<stdio.h>

int main()
{
    char in;
    int cat=0;

    int insertion_flag = 0;
    int insertion_num  = 0;
    int insertion_base = 0;

    int deletion_flag  = 0;
    int deletion_num   = 0;
    int deletion_base  = 0;

    while(scanf("%c",&in)!=EOF)
    {
        cat = 0;
        insertion_flag = 0;
        deletion_flag = 0;

        //find cs:Z:=
        while(cat<5)
        {
            scanf("%c",&in);
            if( in == 'c' )
                cat++;
            else if( in == 's' )
                cat++;
            else if( in == ':' )
                cat++;
            else if( in == 'Z' )
                cat++;
            else
                cat = 0;

        }

        int tmp_in = 0;
        int tmp_dn = 0;

        while( in != '\n' )
        {
            scanf("%c",&in);

            if( in == '+' )
            {
                insertion_flag = 1;
                deletion_flag  = 0;
                insertion_num++;
            }
            else if( in == '-' )
            {
                deletion_flag  = 1;
                insertion_flag = 0;
                deletion_num++;
            }
            else if( in == '=' || in == '*' )
            {
                insertion_flag = 0;
                deletion_flag  = 0;
            }
            else if(insertion_flag)
            {
                insertion_base++;
                tmp_in++;
            }

            else if(deletion_flag)
            {
                deletion_base++;
                tmp_dn++;
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
