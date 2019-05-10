//AUTHOR: Aaron J. Goschie
//DATE: May 10, 2019
#pragma once
extern "C"
{
	__declspec(dllexport) int CreateInterface(bool blocking);
	__declspec(dllexport) int WaitForInterface();
	__declspec(dllexport) int SendData(unsigned char bytes[], unsigned int sizeBytes);
	__declspec(dllexport) unsigned int ReceiveSize(); //need this because you can not create an array in a dll and use it in labview
	__declspec(dllexport) unsigned int ReceiveData(unsigned char bytes[], unsigned int size);
	__declspec(dllexport) int DisconnectInterface();
	__declspec(dllexport) int CloseInterface();
};