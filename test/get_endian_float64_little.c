/* Attempt to read little-endian FLOAT64 */
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
  const char* format_data = "data RAW FLOAT64 1\nENDIAN little\n";
  double u[10];
  double v[20];
  const unsigned char data_data[128 * 8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x14, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x1e, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x26, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x31, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xa1, 0x39, 0x40,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x38, 0x43, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x20, 0xd5, 0x4c, 0x40,
    0x00, 0x00, 0x00, 0x00, 0xd8, 0x9f, 0x55, 0x40,
    0x00, 0x00, 0x00, 0x00, 0xe2, 0x37, 0x60, 0x40,
    0x00, 0x00, 0x00, 0x00, 0xd3, 0x53, 0x68, 0x40,
    0x00, 0x00, 0x00, 0x40, 0xde, 0x3e, 0x72, 0x40,
    0x00, 0x00, 0x00, 0x60, 0x4d, 0x5e, 0x7b, 0x40,
    0x00, 0x00, 0x00, 0x08, 0xba, 0x86, 0x84, 0x40,
    0x00, 0x00, 0x00, 0x0c, 0x17, 0xca, 0x8e, 0x40,
    0x00, 0x00, 0x00, 0x49, 0x91, 0x17, 0x97, 0x40,
    0x00, 0x00, 0xc0, 0xf6, 0xac, 0x51, 0xa1, 0x40,
    0x00, 0x00, 0x20, 0x72, 0x83, 0xfa, 0xa9, 0x40,
    0x00, 0x00, 0x98, 0x95, 0xe2, 0x7b, 0xb3, 0x40,
    0x00, 0x00, 0x64, 0xe0, 0xd3, 0x39, 0xbd, 0x40,
    0x00, 0x00, 0x4b, 0xe8, 0x5e, 0xeb, 0xc5, 0x40,
    0x00, 0x40, 0x38, 0x2e, 0x87, 0x70, 0xd0, 0x40,
    0x00, 0x60, 0x54, 0xc5, 0xca, 0xa8, 0xd8, 0x40,
    0x00, 0x48, 0xff, 0x13, 0x98, 0x7e, 0xe2, 0x40,
    0x00, 0xec, 0xfe, 0x1d, 0xe4, 0xbd, 0xeb, 0x40,
    0x00, 0x31, 0x7f, 0x16, 0x6b, 0xce, 0xf4, 0x40,
    0x80, 0xc9, 0xbe, 0xa1, 0xa0, 0x35, 0xff, 0x40,
    0x20, 0x17, 0x4f, 0x79, 0x38, 0x68, 0x07, 0x41,
    0x58, 0x51, 0xfb, 0x5a, 0x2a, 0x8e, 0x11, 0x41,
    0x04, 0xfa, 0x78, 0x88, 0x3f, 0x55, 0x1a, 0x41,
    0x83, 0xbb, 0x5a, 0xa6, 0xef, 0xbf, 0x23, 0x41,
    0x44, 0x19, 0x88, 0x79, 0xe7, 0x9f, 0x2d, 0x41,
    0xf3, 0x12, 0x26, 0x9b, 0xed, 0x37, 0x36, 0x41,
    0x36, 0x8e, 0x5c, 0x34, 0xf2, 0xa9, 0x40, 0x41,
    0x51, 0xd5, 0x8a, 0x4e, 0xeb, 0xfe, 0x48, 0x41,
    0xfd, 0x1f, 0xe8, 0x7a, 0x30, 0xbf, 0x52, 0x41,
    0xfc, 0x2f, 0x5c, 0xb8, 0xc8, 0x1e, 0x5c, 0x41,
    0xfd, 0x23, 0x45, 0x8a, 0x16, 0x17, 0x65, 0x41,
    0xfc, 0xb5, 0x67, 0xcf, 0xa1, 0xa2, 0x6f, 0x41,
    0x7d, 0xc8, 0x8d, 0x5b, 0xf9, 0xb9, 0x77, 0x41,
    0x5e, 0x56, 0xaa, 0x04, 0x7b, 0xcb, 0x81, 0x41,
    0x8d, 0x81, 0xff, 0x86, 0x38, 0xb1, 0x8a, 0x41,
    0x2a, 0xa1, 0x3f, 0x65, 0xea, 0x04, 0x94, 0x41,
    0xbf, 0x71, 0xdf, 0x97, 0x5f, 0x07, 0x9e, 0x41,
    0x4f, 0x95, 0xe7, 0xb1, 0x87, 0x85, 0xa6, 0x41,
    0xfb, 0xaf, 0x6d, 0xc5, 0x25, 0xe4, 0xb0, 0x41,
    0xf8, 0x87, 0x24, 0xa8, 0x38, 0x56, 0xb9, 0x41,
    0xfa, 0x65, 0x1b, 0x7e, 0xaa, 0x00, 0xc3, 0x41,
    0xf7, 0x18, 0x29, 0xbd, 0xff, 0x80, 0xcc, 0x41,
    0xb9, 0xd2, 0xde, 0xcd, 0xbf, 0x60, 0xd5, 0x41,
    0x0b, 0x1e, 0x67, 0xda, 0x8f, 0x08, 0xe0, 0x41,
    0x10, 0xad, 0x9a, 0xc7, 0xd7, 0x0c, 0xe8, 0x41,
    0xcc, 0x01, 0xb4, 0xd5, 0xa1, 0x09, 0xf2, 0x41,
    0xb2, 0x02, 0x8e, 0xc0, 0x72, 0x0e, 0xfb, 0x41,
    0x06, 0x82, 0x6a, 0x10, 0xd6, 0x4a, 0x04, 0x42,
    0x09, 0xc3, 0x9f, 0x18, 0x41, 0x70, 0x0e, 0x42,
    0x47, 0xd2, 0x77, 0xd2, 0x30, 0xd4, 0x16, 0x42,
    0xb5, 0xdd, 0xd9, 0x9d, 0x24, 0x1f, 0x21, 0x42,
    0x90, 0xcc, 0xc6, 0xec, 0xb6, 0xae, 0x29, 0x42,
    0x6c, 0x19, 0x95, 0x31, 0x09, 0x43, 0x33, 0x42,
    0x22, 0xa6, 0x5f, 0xca, 0x8d, 0xe4, 0x3c, 0x42,
    0x9a, 0xbc, 0xc7, 0x57, 0x6a, 0xab, 0x45, 0x42,
    0x74, 0xcd, 0xd5, 0xc1, 0x8f, 0x40, 0x50, 0x42,
    0x2e, 0xb4, 0xc0, 0xa2, 0xd7, 0x60, 0x58, 0x42,
    0x22, 0x87, 0x10, 0xba, 0xa1, 0x48, 0x62, 0x42,
    0xb3, 0xca, 0x18, 0x97, 0xf2, 0x6c, 0x6b, 0x42,
    0x06, 0x98, 0x52, 0xf1, 0xb5, 0x91, 0x74, 0x42,
    0x09, 0xe4, 0xfb, 0xe9, 0x90, 0xda, 0x7e, 0x42,
    0x07, 0xeb, 0x7c, 0xaf, 0xec, 0x23, 0x87, 0x42,
    0x45, 0xb0, 0x9d, 0x83, 0xf1, 0x5a, 0x91, 0x42,
    0x68, 0x88, 0x6c, 0x45, 0x6a, 0x08, 0x9a, 0x42,
    0x4e, 0x66, 0x11, 0xb4, 0x4f, 0x86, 0xa3, 0x42,
    0x75, 0x19, 0x1a, 0x8e, 0x77, 0x49, 0xad, 0x42,
    0x18, 0x93, 0x93, 0xaa, 0x19, 0xf7, 0xb5, 0x42,
    0x52, 0xae, 0xee, 0x3f, 0x53, 0x79, 0xc0, 0x42,
    0x7b, 0x05, 0xe6, 0xdf, 0xfc, 0xb5, 0xc8, 0x42,
    0x1c, 0x84, 0xec, 0xa7, 0x7d, 0x88, 0xd2, 0x42,
    0x2a, 0xc6, 0xe2, 0x7b, 0xbc, 0xcc, 0xdb, 0x42,
    0xa0, 0x14, 0xea, 0x5c, 0x8d, 0xd9, 0xe4, 0x42,
    0xf0, 0x1e, 0x5f, 0x0b, 0x54, 0x46, 0xef, 0x42,
    0x34, 0x57, 0x87, 0x08, 0xbf, 0x74, 0xf7, 0x42,
    0x67, 0x81, 0x65, 0x46, 0x8f, 0x97, 0x01, 0x43,
    0x1a, 0x42, 0x98, 0xe9, 0x56, 0x63, 0x0a, 0x43,
    0x94, 0x31, 0x32, 0x2f, 0x81, 0xca, 0x13, 0x43,
    0x5e, 0x4a, 0xcb, 0xc6, 0xc1, 0xaf, 0x1d, 0x43,
    0xc6, 0x77, 0x18, 0x55, 0xd1, 0x43, 0x26, 0x43,
    0xd4, 0x59, 0xd2, 0xff, 0xdc, 0xb2, 0x30, 0x43,
    0xbe, 0x86, 0xbb, 0x7f, 0x4b, 0x0c, 0x39, 0x43,
    0x0e, 0xa5, 0xcc, 0x9f, 0x38, 0xc9, 0x42, 0x43,
    0x95, 0xf7, 0xb2, 0xef, 0xd4, 0x2d, 0x4c, 0x43,
    0xb0, 0x39, 0xc6, 0xb3, 0x5f, 0x22, 0x55, 0x43,
    0x88, 0x56, 0xa9, 0x8d, 0x8f, 0xb3, 0x5f, 0x43,
    0xe6, 0x00, 0x3f, 0xaa, 0xab, 0xc6, 0x67, 0x43,
    0xac, 0x40, 0xaf, 0xbf, 0x00, 0xd5, 0x71, 0x43,
    0x02, 0xe1, 0x86, 0x1f, 0x81, 0xbf, 0x7a, 0x43,
    0xc2, 0x28, 0xa5, 0xd7, 0xa0, 0x0f, 0x84, 0x43,
    0x23, 0xbd, 0x77, 0x43, 0x71, 0x17, 0x8e, 0x43,
    0xda, 0xcd, 0x99, 0xf2, 0x94, 0x91, 0x96, 0x43,
    0x64, 0x5a, 0xf3, 0xb5, 0x2f, 0xed, 0xa0, 0x43,
    0x96, 0x07, 0xed, 0x90, 0xc7, 0x63, 0xa9, 0x43,
    0xb0, 0xc5, 0xb1, 0xac, 0xd5, 0x0a, 0xb3, 0x43,
    0x88, 0xa8, 0x0a, 0x83, 0x40, 0x90, 0xbc, 0x43,
    0x66, 0xfe, 0x47, 0x62, 0x30, 0x6c, 0xc5, 0x43,
    0xcc, 0xfe, 0xb5, 0x49, 0x24, 0x11, 0xd0, 0x43,
    0x32, 0xfe, 0x90, 0x6e, 0xb6, 0x19, 0xd8, 0x43,
    0xa6, 0xbe, 0xec, 0xd2, 0x48, 0x13, 0xe2, 0x43,
    0xf9, 0x1d, 0x63, 0x3c, 0xed, 0x1c, 0xeb, 0x43,
    0x7b, 0x56, 0x4a, 0xed, 0xb1, 0x55, 0xf4, 0x43,
    0xb8, 0x81, 0xef, 0xe3, 0x8a, 0x80, 0xfe, 0x43,
    0x4a, 0xa1, 0xf3, 0x2a, 0x68, 0xe0, 0x06, 0x44,
    0xf8, 0xb8, 0x36, 0x20, 0x4e, 0x28, 0x11, 0x44,
    0x74, 0x15, 0x52, 0x30, 0x75, 0xbc, 0x19, 0x44,
    0x17, 0x90, 0x3d, 0xe4, 0x57, 0x4d, 0x23, 0x44,
    0x22, 0x58, 0x5c, 0xd6, 0x03, 0xf4, 0x2c, 0x44,
    0x1a, 0x42, 0xc5, 0xe0, 0x02, 0xb7, 0x35, 0x44,
    0x94, 0xf1, 0x93, 0x28, 0x42, 0x49, 0x40, 0x44,
    0x5e, 0xea, 0xdd, 0x3c, 0xe3, 0x6d, 0x48, 0x44,
    0xc6, 0x6f, 0xa6, 0x6d, 0x6a, 0x52, 0x52, 0x44,
    0xa9, 0xa7, 0x79, 0xa4, 0x9f, 0x7b, 0x5b, 0x44,
    0xbf, 0x3d, 0x5b, 0xbb, 0xb7, 0x9c, 0x64, 0x44,
    0x9e, 0xdc, 0x08, 0x99, 0x13, 0xeb, 0x6e, 0x44,
    0x76, 0xa5, 0xc6, 0xb2, 0x4e, 0x30, 0x77, 0x44,
    0x18, 0xfc, 0x14, 0x06, 0x3b, 0x64, 0x81, 0x44,
    0x24, 0x7a, 0x1f, 0x89, 0x58, 0x16, 0x8a, 0x44,
    0x9b, 0x9b, 0xd7, 0x66, 0xc2, 0x90, 0x93, 0x44,
    0x68, 0x69, 0x43, 0x9a, 0x23, 0x59, 0x9d, 0x44
  };
  int fd, i, r = 0;

  mkdir(filedir, 0777); 

  v[0] = 1.5;
  for (i = 1; i < 20; ++i)
    v[i] = v[i - 1] * 1.5;

  fd = open(format, O_CREAT | O_EXCL | O_WRONLY, 0666);
  write(fd, format_data, strlen(format_data));
  close(fd);

  fd = open(data, O_CREAT | O_EXCL | O_WRONLY | O_BINARY, 0666);
  write(fd, data_data, 128 * sizeof(double));
  close(fd);

  DIRFILE* D = gd_open(filedir, GD_RDONLY | GD_VERBOSE);
  int n = gd_getdata(D, "data", 5, 0, 0, 10, GD_FLOAT64, u);
  int error = gd_error(D);

  gd_close(D);

  unlink(data);
  unlink(format);
  rmdir(filedir);

  CHECKI(error, 0);
  CHECKI(n, 10);

  for (i = 0; i < 10; ++i)
    CHECKFi(i, u[i], v[i + 5]);

  return r;
}