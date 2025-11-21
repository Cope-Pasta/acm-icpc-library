#define ld long double
#define ve vector

using namespace std;
typedef ve<complex<ld>> vc;

const ld pi = acosl(-1);

const int LG = 22;
ve<complex<ld>> w[LG];

void fft(vc &a, int clg)
{
    int n = a.size();
    ve<int> dp(n, 0);
    for (int i = 1; i < n; i++)
        dp[i] = dp[i / 2] / 2 + (((i & 1) << (clg - 1)));
    for (int i = 0; i < n; i++)
        if (i < dp[i])
            swap(a[i], a[dp[i]]);
    for (int i = 1; i <= clg; i++)
    {
        int m = 1 << i;
        for (int j = 0; j < n; j += m)
            for (int k = 0; k < m / 2; k++)
            {
                auto x = a[j + k], y = a[j + k + m / 2];
                a[j + k] = x + w[i][k] * y;
                a[j + k + m / 2] = x - w[i][k] * y;
            }
    }
}

void ifft(vc &a, int clg)
{
    fft(a, clg);
    reverse(a.begin() + 1, a.end());
    for (auto &i : a)
        i /= a.size();
}

ve<ll> multiply(ve<int> a, ve<int> b)
{
    int n = 1, clg = 0;
    int m = a.size() + b.size() - 1;
    while (n < m)
    {
        n *= 2;
        clg++;
    }
    vc ca(n, 0);
    copy(all(a), ca.begin());
    for (int i = 0; i < b.size(); i++)
        ca[i] += complex<ld>(b[i]) * 1il;
    fft(ca, clg);
    for (int i = 0; i < n; i++)
        ca[i] *= ca[i];
    ifft(ca, clg);
    ve<ll> res(m);

    for (int i = 0; i < m; i++)
        res[i] = round(ca[i].imag() / 2);
    return res;
}