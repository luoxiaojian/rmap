#include <stdio.h>
#include <stdlib.h>

// #define USE_RMAP

#ifdef USE_RMAP
#include "rmap.h"
#define MAP rmap
#else
#include <map>
#define MAP std::map
#endif

typedef unsigned vid_t;

int calc_fid_bitwidth(int fnum) {
  int maxfid = fnum - 1;
  int fid_offset;
  if(maxfid == 0) {
    fid_offset = (sizeof(vid_t) * 8) - 1;
  } else {
    int i = 0;
    while(maxfid) {
      maxfid >>= 1;
      i++;
    }
    fid_offset = (sizeof(vid_t) * 8) - i;
  }
  return fid_offset;
}

vid_t get_gid(vid_t *cur, int fid, int fid_offset) {
  vid_t res = fid;
  res = res << fid_offset;
  res += cur[fid];
  ++cur[fid];
  return res;
}

int get_fid(vid_t gid, int fid_offset) {
  gid = gid >> fid_offset;
  int res = gid;
  return res;
} 

int main(int argc, char **argv) {
  if(argc < 4) {
    printf("usage: ./test_twitter <fnum> <in_file> <out_file>\n");
    return 0;
  }
  int fnum = atoi(argv[1]);
  MAP<vid_t, vid_t> m;
  vid_t *cur = (vid_t*)(malloc(sizeof(vid_t) * fnum));
  for(int i = 0; i < fnum; i++) {
    cur[i] = 0;
  }

  printf("begin reading file...\n");

  FILE *fin = fopen(argv[2], "r");
  vid_t oid, gid;
  int fid;
  int fid_offset = calc_fid_bitwidth(fnum);
  while(fscanf(fin, "%u%d", &oid, &fid) != EOF) {
    gid = get_gid(cur, fid, fid_offset);
    m[oid] = gid;
  }
  fclose(fin);

  printf("finished reading file...\n");

  FILE *fout = fopen(argv[3], "wb");
  MAP<vid_t, vid_t>::iterator it;
  for(it = m.begin(); it != m.end(); ++it) {
    fprintf(fout, "%u\t%d\t%u\n", it->first, get_fid(it->second, fid_offset), it->second);
  }
  fflush(fout);
  fclose(fout);

  printf("finished writing file...\n");

  return 0;
}
