int isprime(int i)
{
    int res = 1;
    int del = i;
    int cond = 0;
    int j2 = 4;
    for(int j = 2; j2 <= del; j++)
    {
        cond = i % j;
        if(cond == 0)
        {
            j = del;
            res = 0;
        }
        j = j + 1;
        j2 = j * j;
        j = j - 1;
    }

    return res;
}

int main()
{
    int n;
    int count = 0;
    int isprimenum = 0;
    ReadLine(n);

    for(int i = 2; count < n; i++)
    {
        isprimenum = isprime(i);
        
        if(isprimenum == 1)
        {
            WriteLine(i);
            count = count + 1;
        }
    }
    return 0;
}