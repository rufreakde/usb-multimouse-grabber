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

public:

    Device( size_t _deviceID, std::string _device_name) {
        std::cout << "###### Create Device - 0.0/0.0" << std::endl;
        this->device_id = std::make_shared<size_t>(_deviceID);
        this->device_name = std::make_shared<std::string>(_device_name);
    }

    Device(size_t _deviceID, std::string const& _device_name, float _xStart, float _yStart) : Device(_deviceID, _device_name){
        std::cout << "###### Create Device - " << _xStart << "/" << _yStart << std::endl;
        this->x_previous = _xStart;
        this->y_previous = _yStart;
        this->x_current = _xStart;
        this->y_current = _yStart;
    }

    ~Device(){
        std::cout << "###### Destroyed Device" << std::endl;
    }

    const size_t getId () const{
        return (size_t)this->device_id.get();
    };
    const std::string getName() const{
        return "Not Implementet yet"; //this->device_name.get() does not work because of std::basic_string wtf?
    };

    const float getDeltaX() const{
        return x_previous - x_current;
    };
    const float getDeltaY() const{
        return y_previous - y_current;
    };

private:
    std::shared_ptr<size_t> device_id;
    std::shared_ptr<std::string> device_name;

    float x_previous = 0;
    float y_previous = 0;

    float x_current = 0;
    float y_current = 0;

};

class DevicesManager{

public:
    DevicesManager(){
        std::cout << "###### Created Empty DevicesManager List" << std::endl;
        this->list = std::list<Device>();
    }

    ~DevicesManager(){
        std::cout << "###### Destroyed DevicesManager List" << std::endl;
    }

    std::list<Device> list;

    void getDevicesArray() {

        CFMutableDictionaryRef usb_dictionary;
        io_iterator_t io_device_iterator;
        kern_return_t assembler_kernel_return_value;
        io_service_t device_id;

        // set up a matching dictionary for the class
        usb_dictionary = IOServiceMatching(kIOUSBDeviceClassName);
        if (usb_dictionary == NULL) {
            std::cout << "failed to fetch USB dictionary" << std::endl;
            return; // still empty
        }

        // Now we have a dictionary, get an iterator.
        assembler_kernel_return_value = IOServiceGetMatchingServices(kIOMasterPortDefault, usb_dictionary, &io_device_iterator);
        if (assembler_kernel_return_value != KERN_SUCCESS) {
            std::cout << "failed to get a kern_return" << std::endl;
            return; // still empty
        }

        io_name_t device_name = "unkown device";
        device_id = IOIteratorNext(io_device_iterator); // getting first device

        while (device_id) {

            device_id = IOIteratorNext(io_device_iterator); //set id type: io_service_t
            IORegistryEntryGetName(device_id, device_name); //set name type: io_name_t

            this->list.push_back(Device(device_id, device_name));
        }

        //Done, release the iterator
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
    devices.printDeviceIDs();
    devices.getDevicesArray();
    devices.printDeviceIDs();
}

#endif //CPP_MULTIMOUSE_USBPRIVATEDATASAMPLE_H

//================================================================================================
//
//  custom print device ID to list_ptr device id's
//
//  This function will be called and just lists the USB Device idies connected to the MAC
//  Remove this function later on
//
//================================================================================================
/*void printDeviceIDs() {
    CFMutableDictionaryRef usb_dictionary;
    io_iterator_t io_device_iterator;
    kern_return_t assembler_kernel_return_value;
    io_service_t device_id;

    // set up a matching dictionary for the class
    usb_dictionary = IOServiceMatching(kIOUSBDeviceClassName);
    if (usb_dictionary == NULL) {
        std::cout << "**Failed to retreave a USB dictionary!**" << std::endl;
        return;
    }

    assembler_kernel_return_value = IOServiceGetMatchingServices(kIOMasterPortDefault, usb_dictionary, &io_device_iterator);
    if (assembler_kernel_return_value != KERN_SUCCESS) {
        std::cout << "failed to get a kern_return" << std::endl;
        return;
    }

    std::cout << "IO DevicesManager found:" << std::endl;
    size_t device_number;
    io_name_t device_name;

    while (device_id = IOIteratorNext(io_device_iterator)) {
        // do something with device_id, eg. check properties
        // And free the reference taken before continuing to the next item

        IORegistryEntryGetName(device_id, device_name);

        std::cout << "#" << device_number <<  std::endl;
        std::cout << "| io id: "<< "\t" << device_id <<  std::endl;
        std::cout << "| name: " << "\t" << device_name <<  std::endl;
        std::cout << "#-----------------------------------------------#" << std::endl;

        device_number++;
        IOObjectRelease(device_id);
    }

    //Done, release the iterator
    IOObjectRelease(io_device_iterator);
}*/


