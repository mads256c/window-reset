# window-reset
Moves windows that are not on the primary screen back to the primary screen. Currently only works with windows.

# Downlading
You can download this program from the github actions on this repo. There is both debug and release build for both x86 and x64. You can find them under actions then under artifacts.

# Developing

## Building
Open the solution in Visual Studio (preferably 2019) and click build.

## Running
Run window-reset.exe. The program currently does not support any commandline parameters.
When the program is launched press the hotkey in config.h to move the windows to the primary screen.

## Configuring
Configuring the program happens in config.h. Right now it is only possible to change the hotkey.

# FAQ
## Certain windows like Task Manager does not move
Some programs are protected, to move these programs run window-reset.exe as administrator.
