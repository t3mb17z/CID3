#include <utils/syncsafe.h>

int CID3_SyncSafeToInt(unsigned char *syncsafe) {
  return (
    (syncsafe[0] << 21) |
    (syncsafe[1] << 14) |
    (syncsafe[2] << 7) |
    (syncsafe[3])
  );
};

int CID3_SyncSafeToInt32(unsigned char *syncsafe) {
  return (
    (syncsafe[0] << 24) |
    (syncsafe[1] << 16) |
    (syncsafe[2] << 8) |
    (syncsafe[3])
  );
};
