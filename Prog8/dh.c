#include <stdio.h>

long long mod_exp(long long base, long long exp, long long mod)
{
    long long result = 1;

    while(exp > 0)
    {
        if(exp % 2 == 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}

int main()
{
    long long p, g;
    long long a, b;
    long long A, B;
    long long keyA, keyB;

    printf("Enter the prime no(p): ");
    scanf("%lld", &p);

    printf("Enter the primitive root of p(g): ");
    scanf("%lld", &g);

    printf("Enter Alice private key (a): ");
    scanf("%lld", &a);

    printf("Enter Bob private key (b): ");
    scanf("%lld", &b);

    A = mod_exp(g, a, p);
    B = mod_exp(g, b, p);

    keyA = mod_exp(B, a, p);
    keyB = mod_exp(A, b, p);

    printf("\nAlice Public Key = %lld", A);
    printf("\nBob Public Key = %lld", B);

    printf("\n\nAlice Secret Key = %lld", keyA);
    printf("\nBob Secret Key = %lld\n", keyB);

    return 0;
}
