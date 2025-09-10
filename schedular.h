#pragma once

#include <ocl.h>


cl_event schedule_work(cl_resource* resource, cl_kernel* kernel);
