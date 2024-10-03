#include <stdio.h>

int main(void)
{
  printf("\nchar                  is %d bytes", sizeof(char));
  printf("\nint                   is %d bytes", sizeof(int));
  printf("\nshort                 is %d bytes", sizeof(short));
  printf("\nlong                  is %d bytes", sizeof(long));
  printf("\nlong long             is %d bytes\n", sizeof(long long));

  printf("\nunsigned char         is %d bytes", sizeof(unsigned char));
  printf("\nunsigned int          is %d bytes", sizeof(unsigned int));
  printf("\nunsigned short        is %d bytes", sizeof(unsigned short));
  printf("\nunsigned long         is %d bytes", sizeof(unsigned long));
  printf("\nunsigned long long    is %d bytes\n", sizeof(unsigned long long));

  printf("\nfloat                 is %d bytes", sizeof(float));
  printf("\ndouble                is %d bytes", sizeof(double));
  printf("\nlong double           is %d bytes\n", sizeof(long double));

  return 0;
}
