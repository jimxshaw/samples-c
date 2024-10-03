#include <stdio.h>

int main(void)
{
  printf("\nchar                  is %zu bytes", sizeof(char));
  printf("\nint                   is %zu bytes", sizeof(int));
  printf("\nshort                 is %zu bytes", sizeof(short));
  printf("\nlong                  is %zu bytes", sizeof(long));
  printf("\nlong long             is %zu bytes\n", sizeof(long long));

  printf("\nunsigned char         is %zu bytes", sizeof(unsigned char));
  printf("\nunsigned int          is %zu bytes", sizeof(unsigned int));
  printf("\nunsigned short        is %zu bytes", sizeof(unsigned short));
  printf("\nunsigned long         is %zu bytes", sizeof(unsigned long));
  printf("\nunsigned long long    is %zu bytes\n", sizeof(unsigned long long));

  printf("\nfloat                 is %zu bytes", sizeof(float));
  printf("\ndouble                is %zu bytes", sizeof(double));
  printf("\nlong double           is %zu bytes\n", sizeof(long double));

  return 0;
}
