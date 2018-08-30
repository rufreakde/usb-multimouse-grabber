#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>
#include <iostream>

int main(int argc, const char *argv[])
{
    CFMutableDictionaryRef matchingDict;
    io_iterator_t iter;
    kern_return_t kr;
    io_service_t device;

    // set up a matching dictionary for the class
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (matchingDict == NULL)
    {
        std::cout << "**Failed to retreave a USB dictionary!**" << std::endl;
        return -1; // fail
    }

    // Now we have a dictionary, get an iterator.
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
    if (kr != KERN_SUCCESS)
    {
        std::cout << "success" << std::endl;
        return -1;
    }

    /* iterate */
    while (device = IOIteratorNext(iter))
    {
        // do something with device, eg. check properties
        // And free the reference taken before continuing to the next item
        std::cout << "device iteration:   " << device << std::endl;
        IOObjectRelease(device);
    }

   //Done, release the iterator
   IOObjectRelease(iter);
   return 0;
}