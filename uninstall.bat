%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit

%windir%\system32\regsvr32.exe /u %~dp0x64\Debug\SampleIME.dll
del %~dp0x64\Debug\SampleIMESimplifiedQuanPin.txt
pause