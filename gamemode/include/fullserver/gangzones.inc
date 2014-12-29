/**
The MIT License (MIT)

Copyright (c) 2014 Mateusz Cichon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define GZ_InitGZ(%0) \
  for(new i=0;i<sizeof %0; i++) \
    %0[i][egzgz]=GangZoneCreate( %0[i][egzx1], %0[i][egzy1],%0[i][egzx2],%0[i][egzy2]);

#define GZ_DrawForPlayer(%0,%1) \
  for(new i=0;i<sizeof %1; i++) { \
    GangZoneShowForPlayer(%0,%1[i][egzgz],%1[i][egzcolor]); \
  }

enum gzdef{
  Float:egzx1,Float:egzy1,
  Float:egzx2,Float:egzy2,
  egzcolor, egzgz
}

new gz_arena_aztec[][gzdef]={ // /areny -> /aztec
  {-5033.9, -33.9,  -4966.0, 33.9,  0x000000cc},// aztecall
  {-5026.8, -26.7,  -4973.2, 26.8,  0x30309090},  // azteccenter
  {-5030.4, -4.4, -4969.5, 4.5, 0x90909090},  // aztec1
  {-5004.4, -30.2,  -4995.5, 30.5,  0x90909090}   // aztec2
};

new gz_wyrzutnia[][gzdef]={
  {1361.4, -321.6,  1653.4, -309.6, 0x6d4414FF},  // wyrzutnia1
  {1450.4, -479.0,  1460.1, -146.4, 0x6d4414FF} // wyrzutnia2
};


GZ_Init(){
  GZ_InitGZ(gz_arena_aztec)
  GZ_InitGZ(gz_wyrzutnia)
}
// EOF
