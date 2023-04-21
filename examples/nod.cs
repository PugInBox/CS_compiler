
int getnod(int a, int b)
{
    int res = 0;
    for (int i = a; i > 0; i--)
    {
        res = a % i;
        if (res == 0)
        {
            res = b % i;
            if (res == 0)
            {
                res = i;
                i = 0;
            }
        }
    }
    return res;
}

int main()
{
    int a;
    int b;
    ReadLine(a);
    ReadLine(b);
    int nod = getnod(a, b);
    WriteLine(nod);
    return 0;
}
