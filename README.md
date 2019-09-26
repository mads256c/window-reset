# window-reset
Moves windows that are not on the primary screen back to the primary screen. Currently only works with windows.

## Building
Open the solution in Visual Studio (preferably 2019) and click build.

## Running
Run window-reset.exe. The program currently does not support any commandline parameters.
When the program is launched press the hotkey in config.h to move the windows to the primary screen.

## Configuring
Configuring the program happens in config.h. Right now it is only possible to change the hotkey.

### FAQ
#### Certain windows like Task Manager does not move
Some programs are protected, to move these programs run window-reset.exe as administrator.
