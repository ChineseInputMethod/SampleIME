%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit

%windir%\system32\regsvr32.exe %~dp0x64\Debug\SampleIME.dll
copy %~dp0SampleIME\Dictionary\SampleIMESimplifiedQuanPin.txt %~dp0x64\Debug
pause