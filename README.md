![logo](logo.png)
# CID3
## A simple and fast ID3v2 library written in C

Clone this repository and run

```make
make
make install
```

and it will be done, you can use this library on your proyect now

You can also read the contents to learn how ID3v2 works

You can uninstall by running

```make
make uninstall
```

And it will be deleted from your system

## Usage

```c
#include <stdio.h>
#include <id3/id3.h>

int main() {

    ID3FileRef file;
    ID3readFile("test.mp3", &file);

    for(int i = 0; i < file.tags.count; i++) {
        printf("Frame ID: %s, Content: %s\n", file.tags.tags[i].id, file.tags.tags[i].text);
    }
    
    return 0;
}
```

Expected output:

```
Frame ID: TIT2, Content: Song Title
Frame ID: TPE1, Content: Artist
Frame ID: TALB, Content: Album
Frame ID: TYER, Content: 2021
Frame ID: TRCK, Content: # of #
Frame ID: TCON, Content: Genre
```

