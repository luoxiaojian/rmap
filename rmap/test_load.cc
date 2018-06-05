#include <stdio.h>
#include <stdlib.h>

#include "rmap.h"

typedef unsigned vid_t;

int calc_fid_bitwidth(int fnum) {
  int maxfid = fnum - 1;
  int fid_offset;
  if (maxfid == 0) {
    fid_offset = (sizeof(vid_t) * 8) - 1;
  } else {
    int i = 0;
    while (maxfid) {
      maxfid >>= 1;
      i++;
    }
    fid_offset = (sizeof(vid_t) * 8) - i;
  }
  return fid_offset;
}

int get_fid(vid_t gid, int fid_offset) {
  gid = gid >> fid_offset;
  int res = gid;
  return res;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("usage: ./test_load <fnum> <dump_file> <out_file>\n");
    return 0;
  }

  int fnum = atoi(argv[1]);
  int fid_offset = calc_fid_bitwidth(fnum);

  rmap<vid_t, vid_t> m;

  printf("begin reading dump file...\n");

  m.Load(argv[2]);

  printf("finished reading dump file...\n");

  FILE *fout = fopen(argv[3], "wb");
  rmap<vid_t, vid_t>::iterator it;
  for (it = m.begin(); it != m.end(); ++it) {
    fprintf(fout, "%u\t%d\t%u\n", it->first, get_fid(it->second, fid_offset),
            it->second);
  }
  fflush(fout);
  fclose(fout);

  printf("finished writing file...\n");
}
