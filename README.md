# LabViewInterface
A simple windows API pipe interface, accessible as a DLL for LabView.

Just compile into a DLL.
Use CreateInterface and WaitForInterface to setup a listening pipe connection.
Use ReceiveSize to get the first 4 bytes from the pipe and return its integer value, which corresponds to size of data to Receive.
Use ReceiveData with size you acquired to get the Receive data as a byte array.
Use SendData to send your data as bytes, along with the size of the data.
Use DisconnectInterface and CloseInterface to terminate the pipe connection and delete the pipes.

LabviewInterface is a Dll that is an inteface between LabView and any compatible language 
that can use Windows pipes, as well as convert to the right endianness and variable sizes.
