#pragma once

#include <iostream>

class StorageDevice
{
public:
	StorageDevice();
	~StorageDevice();

	std::string DeviceId;
	int BusType;
	int HealthStatus;
	int SpindleSpeed;
	int MediaType;

};

