#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#include "utils.h"
#include "schroeder_reverberator.h"


int main(int argc, char* argv[]) {
    //ask the user for the parameters
    Parameters params = getUserParameters();

    Schroeder_reverberator *reverb = init_schroeder(params.size, params.diffusion);

    return 0;
}
