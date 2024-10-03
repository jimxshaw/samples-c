#include <stdio.h>

int main(void)
{
  printf("\nchar                  is %lu bytes", sizeof(char));
  printf("\nint                   is %lu bytes", sizeof(int));
  printf("\nshort                 is %lu bytes", sizeof(short));
  printf("\nlong                  is %lu bytes", sizeof(long));
  printf("\nlong long             is %lu bytes\n", sizeof(long long));

  printf("\nunsigned char         is %lu bytes", sizeof(unsigned char));
  printf("\nunsigned int          is %lu bytes", sizeof(unsigned int));
  printf("\nunsigned short        is %lu bytes", sizeof(unsigned short));
  printf("\nunsigned long         is %lu bytes", sizeof(unsigned long));
  printf("\nunsigned long long    is %lu bytes\n", sizeof(unsigned long long));

  printf("\nfloat                 is %lu bytes", sizeof(float));
  printf("\ndouble                is %lu bytes", sizeof(double));
  printf("\nlong double           is %lu bytes\n", sizeof(long double));

  return 0;
}
