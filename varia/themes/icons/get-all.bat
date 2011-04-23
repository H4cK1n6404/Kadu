@ECHO OFF
SETLOCAL EnableDelayedExpansion 

SET AVAILABLE_MODULES=

FOR /D %%I IN (*) DO (
	IF EXIST "%%I/kadu_icons" (
		CALL SET AVAILABLE_MODULES=%%AVAILABLE_MODULES%%;%%I
	)
)

REM When echo called without parameter (or with empty one) it prints its status
REM on screen so we have to avoid that simply adding ; to the end
CALL SET AVAILABLE_MODULES=%%AVAILABLE_MODULES%%;

goto EOF
:append_module_name
set FILE=%~n1
CALL SET AVAILABLE_MODULES=%%AVAILABLE_MODULES%%;%FILE%
exit /b 0
:EOF

ECHO %AVAILABLE_MODULES%
