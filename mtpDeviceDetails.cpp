#include "mtpDeviceDetails.h"
#include <iostream>

mtpDeviceDetails::mtpDeviceDetails() {
	manufacturer="";
	model="";
	version="";
	serialnum="";
	name="";
}

std::string mtpDeviceDetails::toString() {
	std::string dString="Manufacturer:"+manufacturer+";";
	dString.append("Model:"+model+";");
	dString.append("Serial:"+serialnum+";");
	dString.append("Version:"+version+";");
	dString.append("Name:"+name+";");
	return dString;
}

void mtpDeviceDetails::setManufacturer(char *manchar) {
	manufacturer = manchar;
}

void mtpDeviceDetails::setModel(char *modchar) {
	model = modchar;
}

void mtpDeviceDetails::setSerial(char *serchar) {
	serialnum = serchar;
}

void mtpDeviceDetails::setVersion(char *verchar) {
	version = verchar;
}

void mtpDeviceDetails::setName(char *namechar) {
	name = namechar;
}

std::string mtpDeviceDetails::getManufacturer() {
	std::cout << manufacturer << std::endl;
	return manufacturer;
}

std::string mtpDeviceDetails::getModel() {
	std::cout << model << std::endl;
	return model;
}

std::string mtpDeviceDetails::getSerial() {
	std::cout << serialnum << std::endl;
	return serialnum;
}

std::string mtpDeviceDetails::getVersion() {
	std::cout << version << std::endl;
	return version;
}

std::string mtpDeviceDetails::getName() {
	std::cout << name << std::endl;
	return name;
}
