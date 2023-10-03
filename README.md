

Welcome!
This is a utility that can be compiled with Visual Studio 2019 (or newer). The goal of this program is to create a fake SMB Session. The primary purpose of this is to serve as a method to lure attackers into accessing a honey-device. This program comes with no warranty or guarantees. 

### Program Modifications Instructions
This program will require you to modify the code slightly. On line 144, the Windows API CreateProcessWithLogonW API is called, there are two parameters that have been supplied by default - svc-admin (the Username) and contoso.com (the domain). It is necessary that you change these values to something that matches your **production network**.
```
CreateProcessWithLogonW(L"DomainAdminUser", L"YourDomain.com", NULL, LOGON_NETCREDENTIALS_ONLY, <snip>);
```

### Implementation Instructions
After modifying the code and compiling it, you must then install the service. You can do so with the following command:
```
sc create servicename binpath="C:\ProgramData\Services\Inject\service.exe" start="auto"
```

### Verification Steps
To verify the program is functioning correctly, you should check and see what sessions exist on the system. This can be done with the following command:
```
C:\ProgramData\Services\Inject> net sessions
Computer               User name            Client Type       Opens Idle time

-------------------------------------------------------------------------------
\\[::1]                svc-admin                                  0 00:00:04
The command completed successfully.

```

You should check back in about 13 minutes to verify that a new session has been created and the program is working properly.
