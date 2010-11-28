/* Attempt to read big-endian COMPLEX128 */
#include "test.h"

#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
  const char* filedir = __TEST__ "dirfile";
  const char* format = __TEST__ "dirfile/format";
  const char* data = __TEST__ "dirfile/data";
  const char* format_data = "data RAW COMPLEX128 1\nENDIAN big\n";
#ifdef GD_NO_C99_API
  double u[20];
  double v[20][2];
#else
  double complex u[10];
  double complex v[20];
#endif
  const unsigned char data_data[64 * 16] = {
    0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x1e, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x26, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x31, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x39, 0xa1, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x43, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x4c, 0xd5, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x55, 0x9f, 0xd8, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x60, 0x37, 0xe2, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x68, 0x53, 0xd3, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x72, 0x3e, 0xde, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x7b, 0x5e, 0x4d, 0x60, 0x00, 0x00, 0x00,
    0x40, 0x84, 0x86, 0xba, 0x08, 0x00, 0x00, 0x00,
    0x40, 0x8e, 0xca, 0x17, 0x0c, 0x00, 0x00, 0x00,
    0x40, 0x97, 0x17, 0x91, 0x49, 0x00, 0x00, 0x00,
    0x40, 0xa1, 0x51, 0xac, 0xf6, 0xc0, 0x00, 0x00,
    0x40, 0xa9, 0xfa, 0x83, 0x72, 0x20, 0x00, 0x00,
    0x40, 0xb3, 0x7b, 0xe2, 0x95, 0x98, 0x00, 0x00,
    0x40, 0xbd, 0x39, 0xd3, 0xe0, 0x64, 0x00, 0x00,
    0x40, 0xc5, 0xeb, 0x5e, 0xe8, 0x4b, 0x00, 0x00,
    0x40, 0xd0, 0x70, 0x87, 0x2e, 0x38, 0x40, 0x00,
    0x40, 0xd8, 0xa8, 0xca, 0xc5, 0x54, 0x60, 0x00,
    0x40, 0xe2, 0x7e, 0x98, 0x13, 0xff, 0x48, 0x00,
    0x40, 0xeb, 0xbd, 0xe4, 0x1d, 0xfe, 0xec, 0x00,
    0x40, 0xf4, 0xce, 0x6b, 0x16, 0x7f, 0x31, 0x00,
    0x40, 0xff, 0x35, 0xa0, 0xa1, 0xbe, 0xc9, 0x80,
    0x41, 0x07, 0x68, 0x38, 0x79, 0x4f, 0x17, 0x20,
    0x41, 0x11, 0x8e, 0x2a, 0x5a, 0xfb, 0x51, 0x58,
    0x41, 0x1a, 0x55, 0x3f, 0x88, 0x78, 0xfa, 0x04,
    0x41, 0x23, 0xbf, 0xef, 0xa6, 0x5a, 0xbb, 0x83,
    0x41, 0x2d, 0x9f, 0xe7, 0x79, 0x88, 0x19, 0x44,
    0x41, 0x36, 0x37, 0xed, 0x9b, 0x26, 0x12, 0xf3,
    0x41, 0x40, 0xa9, 0xf2, 0x34, 0x5c, 0x8e, 0x36,
    0x41, 0x48, 0xfe, 0xeb, 0x4e, 0x8a, 0xd5, 0x51,
    0x41, 0x52, 0xbf, 0x30, 0x7a, 0xe8, 0x1f, 0xfd,
    0x41, 0x5c, 0x1e, 0xc8, 0xb8, 0x5c, 0x2f, 0xfc,
    0x41, 0x65, 0x17, 0x16, 0x8a, 0x45, 0x23, 0xfd,
    0x41, 0x6f, 0xa2, 0xa1, 0xcf, 0x67, 0xb5, 0xfc,
    0x41, 0x77, 0xb9, 0xf9, 0x5b, 0x8d, 0xc8, 0x7d,
    0x41, 0x81, 0xcb, 0x7b, 0x04, 0xaa, 0x56, 0x5e,
    0x41, 0x8a, 0xb1, 0x38, 0x86, 0xff, 0x81, 0x8d,
    0x41, 0x94, 0x04, 0xea, 0x65, 0x3f, 0xa1, 0x2a,
    0x41, 0x9e, 0x07, 0x5f, 0x97, 0xdf, 0x71, 0xbf,
    0x41, 0xa6, 0x85, 0x87, 0xb1, 0xe7, 0x95, 0x4f,
    0x41, 0xb0, 0xe4, 0x25, 0xc5, 0x6d, 0xaf, 0xfb,
    0x41, 0xb9, 0x56, 0x38, 0xa8, 0x24, 0x87, 0xf8,
    0x41, 0xc3, 0x00, 0xaa, 0x7e, 0x1b, 0x65, 0xfa,
    0x41, 0xcc, 0x80, 0xff, 0xbd, 0x29, 0x18, 0xf7,
    0x41, 0xd5, 0x60, 0xbf, 0xcd, 0xde, 0xd2, 0xb9,
    0x41, 0xe0, 0x08, 0x8f, 0xda, 0x67, 0x1e, 0x0b,
    0x41, 0xe8, 0x0c, 0xd7, 0xc7, 0x9a, 0xad, 0x10,
    0x41, 0xf2, 0x09, 0xa1, 0xd5, 0xb4, 0x01, 0xcc,
    0x41, 0xfb, 0x0e, 0x72, 0xc0, 0x8e, 0x02, 0xb2,
    0x42, 0x04, 0x4a, 0xd6, 0x10, 0x6a, 0x82, 0x06,
    0x42, 0x0e, 0x70, 0x41, 0x18, 0x9f, 0xc3, 0x09,
    0x42, 0x16, 0xd4, 0x30, 0xd2, 0x77, 0xd2, 0x47,
    0x42, 0x21, 0x1f, 0x24, 0x9d, 0xd9, 0xdd, 0xb5,
    0x42, 0x29, 0xae, 0xb6, 0xec, 0xc6, 0xcc, 0x90,
    0x42, 0x33, 0x43, 0x09, 0x31, 0x95, 0x19, 0x6c,
    0x42, 0x3c, 0xe4, 0x8d, 0xca, 0x5f, 0xa6, 0x22,
    0x42, 0x45, 0xab, 0x6a, 0x57, 0xc7, 0xbc, 0x9a,
    0x42, 0x50, 0x40, 0x8f, 0xc1, 0xd5, 0xcd, 0x74,
    0x42, 0x58, 0x60, 0xd7, 0xa2, 0xc0, 0xb4, 0x2e,
    0x42, 0x62, 0x48, 0xa1, 0xba, 0x10, 0x87, 0x22,
    0x42, 0x6b, 0x6c, 0xf2, 0x97, 0x18, 0xca, 0xb3,
    0x42, 0x74, 0x91, 0xb5, 0xf1, 0x52, 0x98, 0x06,
    0x42, 0x7e, 0xda, 0x90, 0xe9, 0xfb, 0xe4, 0x09,
    0x42, 0x87, 0x23, 0xec, 0xaf, 0x7c, 0xeb, 0x07,
    0x42, 0x91, 0x5a, 0xf1, 0x83, 0x9d, 0xb0, 0x45,
    0x42, 0x9a, 0x08, 0x6a, 0x45, 0x6c, 0x88, 0x68,
    0x42, 0xa3, 0x86, 0x4f, 0xb4, 0x11, 0x66, 0x4e,
    0x42, 0xad, 0x49, 0x77, 0x8e, 0x1a, 0x19, 0x75,
    0x42, 0xb5, 0xf7, 0x19, 0xaa, 0x93, 0x93, 0x18,
    0x42, 0xc0, 0x79, 0x53, 0x3f, 0xee, 0xae, 0x52,
    0x42, 0xc8, 0xb5, 0xfc, 0xdf, 0xe6, 0x05, 0x7b,
    0x42, 0xd2, 0x88, 0x7d, 0xa7, 0xec, 0x84, 0x1c,
    0x42, 0xdb, 0xcc, 0xbc, 0x7b, 0xe2, 0xc6, 0x2a,
    0x42, 0xe4, 0xd9, 0x8d, 0x5c, 0xea, 0x14, 0xa0,
    0x42, 0xef, 0x46, 0x54, 0x0b, 0x5f, 0x1e, 0xf0,
    0x42, 0xf7, 0x74, 0xbf, 0x08, 0x87, 0x57, 0x34,
    0x43, 0x01, 0x97, 0x8f, 0x46, 0x65, 0x81, 0x67,
    0x43, 0x0a, 0x63, 0x56, 0xe9, 0x98, 0x42, 0x1a,
    0x43, 0x13, 0xca, 0x81, 0x2f, 0x32, 0x31, 0x94,
    0x43, 0x1d, 0xaf, 0xc1, 0xc6, 0xcb, 0x4a, 0x5e,
    0x43, 0x26, 0x43, 0xd1, 0x55, 0x18, 0x77, 0xc6,
    0x43, 0x30, 0xb2, 0xdc, 0xff, 0xd2, 0x59, 0xd4,
    0x43, 0x39, 0x0c, 0x4b, 0x7f, 0xbb, 0x86, 0xbe,
    0x43, 0x42, 0xc9, 0x38, 0x9f, 0xcc, 0xa5, 0x0e,
    0x43, 0x4c, 0x2d, 0xd4, 0xef, 0xb2, 0xf7, 0x95,
    0x43, 0x55, 0x22, 0x5f, 0xb3, 0xc6, 0x39, 0xb0,
    0x43, 0x5f, 0xb3, 0x8f, 0x8d, 0xa9, 0x56, 0x88,
    0x43, 0x67, 0xc6, 0xab, 0xaa, 0x3f, 0x00, 0xe6,
    0x43, 0x71, 0xd5, 0x00, 0xbf, 0xaf, 0x40, 0xac,
    0x43, 0x7a, 0xbf, 0x81, 0x1f, 0x86, 0xe1, 0x02,
    0x43, 0x84, 0x0f, 0xa0, 0xd7, 0xa5, 0x28, 0xc2,
    0x43, 0x8e, 0x17, 0x71, 0x43, 0x77, 0xbd, 0x23,
    0x43, 0x96, 0x91, 0x94, 0xf2, 0x99, 0xcd, 0xda,
    0x43, 0xa0, 0xed, 0x2f, 0xb5, 0xf3, 0x5a, 0x64,
    0x43, 0xa9, 0x63, 0xc7, 0x90, 0xed, 0x07, 0x96,
    0x43, 0xb3, 0x0a, 0xd5, 0xac, 0xb1, 0xc5, 0xb0,
    0x43, 0xbc, 0x90, 0x40, 0x83, 0x0a, 0xa8, 0x88,
    0x43, 0xc5, 0x6c, 0x30, 0x62, 0x47, 0xfe, 0x66,
    0x43, 0xd0, 0x11, 0x24, 0x49, 0xb5, 0xfe, 0xcc,
    0x43, 0xd8, 0x19, 0xb6, 0x6e, 0x90, 0xfe, 0x32,
    0x43, 0xe2, 0x13, 0x48, 0xd2, 0xec, 0xbe, 0xa6,
    0x43, 0xeb, 0x1c, 0xed, 0x3c, 0x63, 0x1d, 0xf9,
    0x43, 0xf4, 0x55, 0xb1, 0xed, 0x4a, 0x56, 0x7b,
    0x43, 0xfe, 0x80, 0x8a, 0xe3, 0xef, 0x81, 0xb8,
    0x44, 0x06, 0xe0, 0x68, 0x2a, 0xf3, 0xa1, 0x4a,
    0x44, 0x11, 0x28, 0x4e, 0x20, 0x36, 0xb8, 0xf8,
    0x44, 0x19, 0xbc, 0x75, 0x30, 0x52, 0x15, 0x74,
    0x44, 0x23, 0x4d, 0x57, 0xe4, 0x3d, 0x90, 0x17,
    0x44, 0x2c, 0xf4, 0x03, 0xd6, 0x5c, 0x58, 0x22,
    0x44, 0x35, 0xb7, 0x02, 0xe0, 0xc5, 0x42, 0x1a,
    0x44, 0x40, 0x49, 0x42, 0x28, 0x93, 0xf1, 0x94,
    0x44, 0x48, 0x6d, 0xe3, 0x3c, 0xdd, 0xea, 0x5e,
    0x44, 0x52, 0x52, 0x6a, 0x6d, 0xa6, 0x6f, 0xc6,
    0x44, 0x5b, 0x7b, 0x9f, 0xa4, 0x79, 0xa7, 0xa9,
    0x44, 0x64, 0x9c, 0xb7, 0xbb, 0x5b, 0x3d, 0xbf,
    0x44, 0x6e, 0xeb, 0x13, 0x99, 0x08, 0xdc, 0x9e,
    0x44, 0x77, 0x30, 0x4e, 0xb2, 0xc6, 0xa5, 0x76,
    0x44, 0x81, 0x64, 0x3b, 0x06, 0x14, 0xfc, 0x18,
    0x44, 0x8a, 0x16, 0x58, 0x89, 0x1f, 0x7a, 0x24,
    0x44, 0x93, 0x90, 0xc2, 0x66, 0xd7, 0x9b, 0x9b,
    0x44, 0x9d, 0x59, 0x23, 0x9a, 0x43, 0x69, 0x68
  };
  int fd, i, n, error, r = 0;
  DIRFILE *D;

  mkdir(filedir, 0777); 

#ifdef GD_NO_C99_API
  v[0][0] = 1.5;
  v[0][1] = 2.25;
  for (i = 1; i < 20; ++i) {
    v[i][0] = v[i - 1][0] * 2.25;
    v[i][1] = v[i - 1][1] * 2.25;
  }
#else
  v[0] = 1.5 + _Complex_I * 2.25;
  for (i = 1; i < 20; ++i)
    v[i] = v[i - 1] * 2.25;
#endif

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  fd = open(data, O_CREAT | O_EXCL | O_WRONLY | O_BINARY, 0666);
  write(fd, data_data, 64 * 16 * sizeof(unsigned char));
  close(fd);

  D = gd_open(filedir, GD_RDONLY | GD_VERBOSE);
  n = gd_getdata(D, "data", 5, 0, 0, 10, GD_COMPLEX128, u);
  error = gd_error(D);

  gd_close(D);

  unlink(data);
  unlink(format);
  rmdir(filedir);

  CHECKI(error, 0);
  CHECKI(n, 10);

  for (i = 0; i < 10; ++i)
#ifdef GD_NO_C99_API
    CHECKCi(i, u + 2 * i, v[i + 5]);
#else
    CHECKCi(i, u[i], v[i + 5]);
#endif

  return r;
}
