/*
 * Display Device Information
 *
 * Script to print out some information about the OpenCL devices
 * and platforms available on your system
 *
 * History: C++ version written by Tom Deakin, 2012
 *          Updated by Tom Deakin, August 2013
*/

#define __CL_ENABLE_EXCEPTIONS

#include "cl.hpp"
#include <iostream>
#include <vector>

char* err_code(cl_int);

int main(void)
{

  try
  {
    // Discover number of platforms
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::cout << "\nNumber of OpenCL plaforms: " << platforms.size() << std::endl;

    // Investigate each platform
    std::cout << "\n-------------------------" << std::endl;
    for (int i=0; i<platforms.size(); i++) //cl::Platform& plat : platforms)
    {
      cl::Platform& plat = platforms[i];      

      std::string s;
      plat.getInfo(CL_PLATFORM_NAME, &s);
      std::cout << "Platform: " << s << std::endl;

      plat.getInfo(CL_PLATFORM_VENDOR, &s);
      std::cout << "\tVendor:  " << s << std::endl;

      plat.getInfo(CL_PLATFORM_VERSION, &s);
      std::cout << "\tVersion: " << s << std::endl;

      // Discover number of devices
      std::vector<cl::Device> devices;
      plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
      std::cout << "\n\tNumber of devices: " << devices.size() << std::endl;

      // Investigate each device
      for (int j=0; j<devices.size(); j++)//cl::Device& dev : devices)
      {
        cl::Device& dev = devices[j];
        std::cout << "\t-------------------------" << std::endl;

        dev.getInfo(CL_DEVICE_NAME, &s);
        std::cout << "\t\tName: " << s << std::endl;

        dev.getInfo(CL_DEVICE_OPENCL_C_VERSION, &s);
        std::cout << "\t\tVersion: " << s << std::endl;

        int i;
        dev.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &i);
        std::cout << "\t\tMax. Compute Units: " << i << std::endl;

        size_t size;
        dev.getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &size);
        std::cout << "\t\tLocal Memory Size: " << size/1024 << " KB" << std::endl;

        dev.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &size);
        std::cout << "\t\tGlobal Memory Size: " << size/(1024*1024) << " MB" << std::endl;

        dev.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &size);
        std::cout << "\t\tMax Alloc Size: " << size/(1024*1024) << " MB" << std::endl;

        dev.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
        std::cout << "\t\tMax Work-group Size: " << size << std::endl;

        std::vector<size_t> d;
        dev.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &d);
        std::cout << "\t\tMax Work-item Dims: (";
        for (int k=0; k<d.size(); k++)
          std::cout << d[k] << " ";
        std::cout << "\x08)" << std::endl;

        std::cout << "\t-------------------------" << std::endl;

      }

      std::cout << "\n-------------------------\n";
    }

  }
  catch (cl::Error err)
  {
    std::cout << "OpenCL Error: " << err.what() << " returned " << err_code(err.err()) << std::endl;
    std::cout << "Check cl.h for error codes." << std::endl;
    exit(-1);
  }

  return 0;

}
