// UEFI From Scratch Tutorials - ThatOSDev ( 2021 )
// https://github.com/ThatOSDev/UEFI-Tuts

#include "efi.h"
#include "ErrorCodes.h"
#include "efilibs.h"

// This is like int main() in a typical C program.
// In this case, we create an ImageHandle for the overall EFI interface,
// as well as a System Table pointer to the EFI_SYSTEM_TABLE struct.
// UEFI 2.9 Specs PDF Page 91
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST)
{
    // We setup this global variable in the libs.h file.
    SystemTable = ST;
    
    ResetScreen();
    
    SetColor(EFI_WHITE);
    SetTextPosition(3, 2);
    Print(L"Welcome to the UEFI From Scratch Tutorials\r\n\r\n");
    
    SetColor(EFI_GREEN);
    SetTextPosition(8, 4);
    Print(L"Hit Any Key to see Graphics");

    HitAnyKey();
    
    Print(L"\r\n\r\nLoading Graphics Output Protocol ... ");
    EFI_STATUS Status = SystemTable->BootServices->LocateProtocol(&EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, 0, (void**)&gop);
    if(Status == EFI_SUCCESS)
    {
        SetColor(EFI_CYAN);
        Print(CheckStandardEFIError(Status));
        SetGraphicsColor(ORANGE);
        CreateFilledBox(50, 50, 100, 200);
        SetGraphicsColor(RED);
        CreateFilledBox(60, 60, 80, 30);
        SetColor(EFI_YELLOW);
        SetTextPosition(5, 8);
        Print(L"We have Graphics !!");
    } else {
        SetColor(EFI_RED);
        Print(CheckStandardEFIError(Status));
    }

    SetColor(EFI_MAGENTA);

    Print(L"\r\n");
    for(UINTN i = 0; i < 60; i++)
    {
        Delay(18);      // This is in the efilibs.h. It delays the CPU for 18 milliseconds.
        Print(L".");
    } 

    SetColor(EFI_LIGHTGRAY);
    SetTextPosition(5, 9);
    Print(L"We Delayed 18 milliseconds per dot");

    SetColor(EFI_GREEN);
    SetTextPosition(2, 10);
    Print(L"Hit q to quit");

    SetColor(EFI_WHITE);
    EFI_INPUT_KEY GetKey;
	UINTN u = 0;
	unsigned int x = 5;
	BOOLEAN y = 1;
	SystemTable->ConIn->Reset(SystemTable->ConIn, 1);
    while(1)
    {
		Delay1();
		u++;
		if(u > 36000)   // This is 36 * 1000 ==> 36 milliseconds
		{
			u = 0;
			SetTextPosition(x, 17);
			Print(L"   ...   ");
			if(y == 1)
			{
			    x++;
			} else {
				x--;
			}
			if(x < 5)
			{
				x = 5;
				y = 1;
			} else if(x > 50)
			{
				x = 50;
				y = 0;
			}
		}
        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &GetKey);
        if(GetKey.UnicodeChar == 'q')
		{
			break;
        }
    }

    COLD_REBOOT();

    // We should not make it to this point.
    return EFI_SUCCESS;
}