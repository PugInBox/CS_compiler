
int func(int i)
{
    WriteLine(i);
    i = i + 2022;
    WriteLine(i);
    return i;
}

int main()
{
    int a = 10;
    int b = a;
    int c = a + b;
    WriteLine(c);
    float fa = 2.1;
    float fb = fa;
    float fc = fa + fb;
    WriteLine(fc);
    c = c - 12;
    WriteLine(c);
    fc = fb - 14.7;
    WriteLine(fc);
    c = c / 2;
    WriteLine(c);
    c = c % 3;
    WriteLine(c);
    c = 42 + 42;
    WriteLine(c);
    c = 42 + a;
    WriteLine(c);
    if(c > 51)
    {
        c = 0;
        WriteLine(c);
    } else {
        c = 1;
        WriteLine(c);
    }
    for(int i = 0; i < 5; i++)
    {
        WriteLine(i);
    }

    for(int i = 0; i < 5; i++)
    {
        i = i + 14;
        WriteLine(i);
    }
    int res = func(c);
    WriteLine(res);

    return a;
}