//
// Created by Rudolf Chrispens on 30.05.18.
//

#ifndef CPP_MULTIMOUSE_USBPRIVATEDATASAMPLE_H
#define CPP_MULTIMOUSE_USBPRIVATEDATASAMPLE_H

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <iostream>
#include <list>

class Device;

class Device {

private:
    size_t device_id;
    std::string device_name;

    float x_previous = 0;
    float y_previous = 0;

    float x_current = 0;
    float y_current = 0;

public:

    Device( size_t _deviceID, std::string _device_name) {
        //std::cout << "###### Create Device - 0.0/0.0" << std::endl;
        this->device_id = _deviceID;
        this->device_name = _device_name;
    }

    Device(size_t _deviceID, std::string const& _device_name, float _xStart, float _yStart) : Device(_deviceID, _device_name){
        //std::cout << "###### Create Device - " << _xStart << "/" << _yStart << std::endl;
        this->x_previous = _xStart;
        this->y_previous = _yStart;
        this->x_current = _xStart;
        this->y_current = _yStart;
    }

    ~Device(){
        //std::cout << "###### Destroyed Device" << std::endl;
    }

    const size_t getId () const{
        return this->device_id;
    };
    const std::string getName() const{
        return this->device_name;
    };

    const float getDeltaX() const{
        return x_previous - x_current;
    };
    const float getDeltaY() const{
        return y_previous - y_current;
    };
};

class DevicesManager{

private:
    std::list<Device> list = std::list<Device>();

public:
    DevicesManager(){
        //std::cout << "###### Created Empty DevicesManager List" << std::endl;
    }

    ~DevicesManager(){
        //std::cout << "###### Destroyed DevicesManager List" << std::endl;
    }

    void getDevicesArray() {

        CFMutableDictionaryRef usb_dictionary;
        io_iterator_t io_device_iterator;
        kern_return_t assembler_kernel_return_value;
        io_service_t device_id;

        // set up a matching dictionary for the class
        usb_dictionary = IOServiceMatching(kIOUSBDeviceClassName);
        if (usb_dictionary == NULL) {
            std::cout << "failed to fetch USB dictionary" << std::endl;
            return;
        }

        // now we have a dictionary, get an iterator.
        assembler_kernel_return_value = IOServiceGetMatchingServices(kIOMasterPortDefault, usb_dictionary, &io_device_iterator);
        if (assembler_kernel_return_value != KERN_SUCCESS) {
            std::cout << "failed to get a kern_return" << std::endl;
            return;
        }

        io_name_t device_name = "unkown device";

        device_id = IOIteratorNext(io_device_iterator);
        while (device_id) {

            // set name
            IORegistryEntryGetName(device_id, device_name);
            // add device
            this->list.emplace_back(device_id, device_name);
            // next id
            device_id = IOIteratorNext(io_device_iterator);
        }

        // release the iterator
        IOObjectRelease(io_device_iterator);
    }

    void printDeviceIDs(){

        for (auto const& device : this->list) {
            std::cout << "#" << device.getId() <<  std::endl;
            std::cout << "| name: " << "\t" << device.getName() <<  std::endl;
            std::cout << "#-----------------------------------------------#" << std::endl;
        }
    }
};

int main(int argc, const char *argv[])
{
    DevicesManager devices;
    devices.getDevicesArray();
    devices.printDeviceIDs();

}

#endif //CPP_MULTIMOUSE_USBPRIVATEDATASAMPLE_H