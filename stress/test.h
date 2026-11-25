// Unit-test helpers for stress/*.cpp: EXPECT(cond) fails the test (returns 1 from the enclosing int function).
#define EXPECT(cond) do { if (!(cond)) { printf("%s:%d: expected %s\n", __FILE__, __LINE__, #cond); return 1; } } while (0)
inline bool near(double a, double b, double e = 1e-6) { return fabs(a - b) < e; }
