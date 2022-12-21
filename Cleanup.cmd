DEL /Q "%~dp0/Inheritors.sln"

DEL /S/Q "%~dp0/Binaries"
RMDIR /S/Q "%~dp0/Binaries"

DEL /S/Q "%~dp0/.vs"
RMDIR /S/Q "%~dp0/.vs"

DEL /S/Q "%~dp0/DerivedDataCache"
RMDIR /S/Q "%~dp0/DerivedDataCache"

DEL /S/Q "%~dp0/Intermediate"
RMDIR /S/Q "%~dp0/Intermediate"

DEL /S/Q "%~dp0/Saved"
RMDIR /S/Q "%~dp0/Saved"