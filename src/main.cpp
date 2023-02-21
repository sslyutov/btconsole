/*!\brief bluetooth utility
* \file main.cpp
* \date Fenbuary 19, 2023
* \author Sergey Slyutov
*/

#include <iostream>

#include <qversionnumber>

#include <QBluetoothLocalDevice>

#include <QBluetoothDeviceDiscoveryAgent>

#include <QFuture>

#include <QtConcurrent/QtConcurrent>

#include <QApplication>

#include <QWidget>

#include <QLowEnergyController>



int main (int argc, char* argv[])
{
	QApplication app(argc, argv);

	std::cout << "Bluetooth utility" << " Build date " << _CRT_STRINGIZE(__DATE__) << std::endl;
	
	std::cout << "Qt version " << QString("%1.%2.%3").arg(QT_VERSION_MAJOR).arg(QT_VERSION_MINOR).arg(QT_VERSION_PATCH).toStdString() << std::endl << std::endl;
	
	std::cout << "all local devices" << std::endl;

	for (auto localdev : QBluetoothLocalDevice::allDevices()) {
		std::cout << "\t" << "address " << localdev.address().toString().toStdString() << std::endl;
		std::cout << "\t" << "name " << localdev.name().toStdString() << std::endl << std::endl;
	}

	QBluetoothLocalDevice btlocaldev;
	std::cout << std::endl;
	std::cout << "current local BT device " << std::endl;
	std::cout << "\t" << "address " << btlocaldev.address().toString().toStdString() << std::endl;
	std::cout << "\t" << "name " << btlocaldev.name().toStdString() << std::endl << std::endl;

	// ========== prepare host device =============
	if (!btlocaldev.isValid()) {
		std::cout << "current local BT device in invalid" << std::endl;
		return 0;
	}

	std::string strin;
	while( btlocaldev.HostPoweredOff() ) {
		std::cout << "The host is powered Off. Press anykey to power On the host" << std::endl;
		std::cin >> strin;
		btlocaldev.powerOn();
	}

	std::cout << "The host is powered On" << std::endl << std::endl;

	std::cout << "Host mode ";
	switch (btlocaldev.hostMode()) {
		case QBluetoothLocalDevice::HostPoweredOff:
			std::cout << "HostPoweredOff" << std::endl;
			break;
		case QBluetoothLocalDevice::HostConnectable:
			std::cout << "HostConnectable" << std::endl;
			break;
		case QBluetoothLocalDevice::HostDiscoverable:
			std::cout << "HostDiscoverable" << std::endl;
			break;
		case QBluetoothLocalDevice::HostDiscoverableLimitedInquiry:
			std::cout << "HostDiscoverableLimitedInquiry" << std::endl;
			break;
	}
	
	//QBluetoothDeviceDiscoveryAgent btdiscoverAgent(btlocaldev.address());
	//QFuture ft = QtConcurrent::run([&btlocaldev, &btdiscoverAgent]() {
		QEventLoop loop;
		QBluetoothDeviceDiscoveryAgent btdiscoverAgent(btlocaldev.address());
		int devselection = -1;
		bool bcon = QObject::connect(&btdiscoverAgent, &QBluetoothDeviceDiscoveryAgent::finished, &loop, &QEventLoop::quit);
		bcon = QObject::connect(&btdiscoverAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, &loop, &QEventLoop::quit);
		bcon = QObject::connect(&btdiscoverAgent, &QBluetoothDeviceDiscoveryAgent::canceled, &loop, &QEventLoop::quit);
		bcon = QObject::connect(&btdiscoverAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, &loop, [&devselection, &btdiscoverAgent](const QBluetoothDeviceInfo& info) {
			std::cout << "\t" << ++devselection << " " << info.address().toString().toStdString() << " " << info.name().toStdString() << std::endl;
			if(info.address().toString().contains("8C:F6:81"))
				btdiscoverAgent.stop();
		});
		btdiscoverAgent.start(QBluetoothDeviceDiscoveryAgent::DiscoveryMethod::LowEnergyMethod);
		std::cout << "discovered devices" << std::endl;
		loop.exec();
	//});

	//ft.waitForFinished();


	std::cout << " devices discovering completed" << std::endl;
	std::cout << " enter device number to connect" << std::endl;
	QLowEnergyController* plec = NULL;
	while (std::cin >> strin) {
		int devselection = atoi(strin.c_str());
		
		QEventLoop conloop;

		if (devselection < btdiscoverAgent.discoveredDevices().count() && devselection >= 0) {
			const QBluetoothDeviceInfo btdevinfo = btdiscoverAgent.discoveredDevices().at(devselection);

			plec = QLowEnergyController::createCentral(btdevinfo, btlocaldev.address());

			
			bool bcon = QObject::connect(plec, &QLowEnergyController::connected, &conloop, [=, &conloop]() {
				std::cout << "connected " << plec->localAddress().toString().toStdString() << " " << plec->remoteAddress().toString().toStdString() << std::endl;
				conloop.quit();
			});
			bcon = QObject::connect(plec, &QLowEnergyController::disconnected, &conloop, [=, &conloop]() {
				std::cout << "disconnected " << plec->localAddress().toString().toStdString() << " " << plec->remoteAddress().toString().toStdString() << std::endl;
				//conloop.quit();
			});
			bcon = QObject::connect(plec, &QLowEnergyController::errorOccurred, &conloop, [=, &conloop](QLowEnergyController::Error newError){
				std::cout << "error " << newError << " " << plec->localAddress().toString().toStdString() << " " << plec->remoteAddress().toString().toStdString() << std::endl;
				//conloop.quit();
			});

			plec->connectToDevice();
			conloop.exec();
		}
	}
	
	//
	//	switch (btlocaldev.pairingStatus(connAddr)) {
	//		case QBluetoothLocalDevice::Unpaired		 :
	//			std::cout << "device " << connAddr.toString().toStdString() << " Unpaired";
	//			break;
	//		case QBluetoothLocalDevice::Paired			 :
	//			std::cout << "device " << connAddr.toString().toStdString() << " Paired";
	//			break;
	//		case QBluetoothLocalDevice::AuthorizedPaired :
	//			std::cout << "device " << connAddr.toString().toStdString() << " AuthorizedPaired";
	//			break;
	//	}
	//}

	return 0;
}