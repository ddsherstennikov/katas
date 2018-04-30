int median(int a, int b, int c)
{
    return (
        (a>b) ? 
            (a>c ? 
                (b>c ? 
                    b : 
                        c) : 
                            a) : 
        (b>c ? 
            (a>c ? 
                a : 
                    c) : 
                        b)
           );
}

int reference(int a, int b, int c)
{
    int v[3] = {a, b, c};
    std::sort(v, v + 3);
    return v[1];
}
