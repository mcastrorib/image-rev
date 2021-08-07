// include CMake Configuration file
#include "imagerev_config.h"

// include built-in PathFinder library
#include "ImageREVHeader.h"

// Main Program
int main(int argc, char *argv[])
{        
    ImageREVApp *app;
    app = new ImageREVApp(INPUT_FILE);
    app->run();
    
    delete app;
    app = NULL;
    return 0;
}
