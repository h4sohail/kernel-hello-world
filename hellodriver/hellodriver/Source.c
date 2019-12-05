#include "ntddk.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\hellodriver");
UNICODE_STRING SymLinkName = RTL_CONSTANT_STRING(L"\\??\\hellodriversymlink");
PDEVICE_OBJECT DeviceObject = NULL;

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	IoDeleteSymbolicLink(&SymLinkName);
	IoDeleteDevice(DeviceObject);

	DbgPrint("Driver unloaded succesfully \r\n");
}

NTSTATUS DispatchPassThru(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_SUCCESS;

	switch (irpsp->MajorFunction) {
	case IRP_MJ_CREATE:
		DbgPrint("[Handle]: Created \r\n");
		break;
	case IRP_MJ_CLOSE:
		DbgPrint("[Handle]: Closed \r\n");
		break;
	case IRP_MJ_READ:
		DbgPrint("[Handle]: Read \r\n");
		break;
	default:
		break;
	}

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;

	DriverObject->DriverUnload = Unload;

	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);

	if (!NT_SUCCESS(status)) {
		DbgPrint("Device creation failed \r\n");
		return status;
	}

	status = IoCreateSymbolicLink(&SymLinkName, &DeviceName);
	
	if (!NT_SUCCESS(status)) {
		DbgPrint("Symbolic link creation failed \r\n");
		IoDeleteDevice(DeviceObject);
		return status;
	}

	for (int i = 0; i<IRP_MJ_MAXIMUM_FUNCTION; i++) {
		DriverObject->MajorFunction[i] = DispatchPassThru;
	}

	DbgPrint("Driver loaded succesfully \r\n");

	return status;
}