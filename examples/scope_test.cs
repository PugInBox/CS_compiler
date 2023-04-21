
int a = 243;
int j = 5;
int sm;
int func(int i, float f, char c) {
    string funcprint = "func START";
    WriteLine(funcprint);
    int a;
    a = 151;
    int smain;
    smain = i;
    WriteLine(a);
    WriteLine(i);
    WriteLine(f);
    WriteLine(c);
    string funcprintend = "func END";
    WriteLine(funcprintend);
    return 0;
}

float main()
{
    int sm = 3;
    int k = 15;
    int l = 5;
    char fc = 'A';
    float ff = 24.2;
    k = sm + 1;
    for(sm; sm < k; sm++)
    {
        string message = "Global a";
        WriteLine(message);
        WriteLine(a);
        a = 1;
        int b = 2;
        int c = 3;
        int j = 1;
        if(sm == 3)
        {
           b = 5;
           int b1 = 4;
           for(int b2 = 6; b2 > b1; b2--)
           {
                l = l + b2;
                string messagefor = "Local l";
                WriteLine(messagefor);
                WriteLine(l);
           }
           j = 4444;
        }
        string message1 = "Local j";
        WriteLine(message1);
        WriteLine(j);
        int jk = 2;
        
        b = 5;
    }
    string message3 = "Global j";
    WriteLine(message3);
    WriteLine(j);
    for(sm; sm < 51; sm++)
    {
        int e = 6;
        int f = 7;
        j = 8;
    }
    WriteLine(message3);
    WriteLine(j);
    if(sm == 2)
    {
        int ifh = 242;
    } else {
        int ifh = 2;
    }
    func(k, ff, fc);
    string message4 = "Global a";
    WriteLine(message4);
    WriteLine(a);
    return 0.0;
}