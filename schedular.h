#pragma once

#include <ocl.h>

// the work schedular only exists to track
// the progress of the rendering
// the image is divided into tiles of 8x8 pixels
// and 10% of tiles are rendered at once

// all the tiles can be rendered at once however
// there is a negligible performance impact
// when doing 10% at once

cl_event schedule_work(cl_resource* resource, cl_kernel* kernel);
