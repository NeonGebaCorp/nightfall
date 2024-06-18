# Nightfall Framework

Nightfall is a lightweight C framework providing basic system operations and text output functionalities.

## Features

Nightfall includes the following functions:

- **aprint(const char *message)**: Prints a message to the console.
- **areboot()**: Reboots the system.
- **apoweroff()**: Powers off the system.
- **abootable()**: Sets a bootable signature.
- **asetgraphics(int width, int height)**: Sets graphics mode with specified dimensions.
- **acolor(const char *hex)**: Sets text color using a hexadecimal color code.
- **acli()**: Clears interrupt flag.
- **initfs()**: Initializes a simple filesystem.

## Getting Started

### Installation

Clone the Nightfall repository:

```bash
git clone https://github.com/NeonGebaCorp/nightfall.git
cd nightfall
```

### Compiling Nightfall

Compile Nightfall using GCC:

```bash
gcc -o nightfall nightfall.c
```

### Example Usage

Create a C program (`example.c`) that uses Nightfall functions:

```c
#include <stdio.h>
#include "nightfall.h"

int main() {
    // Print a message
    aprint("Welcome to Nightfall!");

    // Set graphics mode
    asetgraphics(80, 25);

    // Set text color
    acolor("#808080"); // Light grey

    // Perform system operations
    abootable();
    acli();

    return 0;
}
```

Compile the example program:

```bash
gcc -o example example.c nightfall.c
```

Run the example program:

```bash
./example
```

## Contributing

Contributions to Nightfall are welcome! Fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
