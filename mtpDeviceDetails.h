#include <string>

class mtpDeviceDetails {
	private:
		std::string manufacturer;
		std::string model;
		std::string serialnum;
		std::string version;
		std::string name;
		
	public:
		void setManufacturer(char *);
		void setModel(char *);
		void setSerial(char *);
		void setVersion(char *);
		void setName(char *);
		std::string getManufacturer();
		std::string getModel();
		std::string getSerial();
		std::string getVersion();
		std::string getName();
		mtpDeviceDetails();
		std::string toString();
};
