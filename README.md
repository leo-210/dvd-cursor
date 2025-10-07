# DVD-Cursor

Make your cursor follow the famous DVD Logo animation when you're not using it !

(Linux only, on any distro and any graphic environnement).


## Compiling

> Note : you might need to change the configuration file for your screen's resolution ! See the next section.

Clone the repo, and then run :
```
make
```

The binary will be located at `./bin/dvd-cursor`

To run it, you need to use `sudo`, to allow for moving the mouse and detecting its movements.

Alternatively, to avoid giving unnecessary permissions, you can run it as a user in the `input` and `uinput` groups.


## Configuration

You can configure the program with the `src/config.h` file. You need to recompile the program to apply the changes.

### Default configuration file :
```c
// src/config.h

#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 1920
#define HEIGHT 1080

// In pixels per 20 ms
// More than 2px/20ms could make movement less precise
#define SPEED 2

// Cooldown before starting animation, in seconds
#define COOLDOWN 5

#endif // CONFIG_H
```
