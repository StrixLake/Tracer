## Ray Tracer in OpenCL

### cl_resource
The OpenCL resources are managed by the cl_resource class which contains the device, its assisiated context and command queue, and the compiled kernel program. Any function that needs to use the OpenCL runtime accepts the cl_resource object by pointer

![](rendered.bmp)