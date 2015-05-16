!include "MUI.nsh"

!define VERSION "1.4.0"

RequestExecutionLevel admin

Name "FullServer.eu Launcher ${VERSION}"
OutFile "fsLauncher-${VERSION}-install.exe"
AutoCloseWindow true
DirText "Wybierz sciezke instalacji GTA San Andreas i SA-MP"
InstallDir "$PROGRAMFILES\Rockstar Games\GTA San Andreas\"
InstallDirRegKey HKLM "Software\Rockstar Games\GTA San Andreas\Installation" ExePath

!define MUI_ABORTWARNING

!define MUI_WELCOMEPAGE_TITLE "Witaj!"
!define MUI_FINISHPAGE_TITLE "Instalacja zakonczona!"

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "Polish"

Function .onVerifyInstDir
	IfFileExists $INSTDIR\gta_sa.exe CheckSamp
		Abort
  CheckSamp:
  IfFileExists $INSTDIR\samp.dll StartInstall
    Abort
	StartInstall:
FunctionEnd

Function CheckRedistributableInstalled
  Push $R0
  ClearErrors

  ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{13A4EE12-23EA-3371-91EE-EFB36DDFFF3E}" "Version"

  IfErrors 0 NoErrors
  StrCpy $R0 "Error"

  NoErrors:
  Exch $R0 
FunctionEnd

Section "" 
	SetOutPath $INSTDIR
	File Release\error.htm
	File Release\load.htm
	File Release\fsLauncher.exe
	File Release\Interop.SHDocVw.1.1.dll
	File Release\news_feed_background.png
   File fsLauncher\vcredist_x86.exe

   Call  CheckRedistributableInstalled
   Pop $R0
 
   ${If} $R0 == "Error"
      ExecWait '"$INSTDIR\vcredist_x86.exe"  /passive /norestart'
   ${EndIf}

	 SetOutPath $INSTDIR
	 WriteUninstaller fsLauncher-uninstall.exe
	
	 CreateDirectory "$SMPROGRAMS\FullServer.eu - Launcher"
	 CreateShortcut "$SMPROGRAMS\FullServer.eu - Launcher\Uruchom Launcher.lnk" "$INSTDIR\fsLauncher.exe"
	 CreateShortcut "$SMPROGRAMS\FullServer.eu - Launcher\Odinstaluj.lnk" "$INSTDIR\fsLauncher-uninstall.exe"
    
    Delete $INSTDIR\vcredist_x86.exe
SectionEnd

Section "Uninstall"
	Delete $INSTDIR\fsLauncher.exe
	Delete $INSTDIR\load.htm
	Delete $INSTDIR\error.htm
	Delete $INSTDIR\Interop.SHDocVw.1.1.dll
	Delete $INSTDIR\news_feed_background.png
   Delete $INSTDIR\fsLauncher-uninstall.exe
   Delete $INSTDIR\vcredist_x86.exe
	
	Delete "$SMPROGRAMS\FullServer.eu - Launcher\Uruchom Launcher.lnk"
	Delete "$SMPROGRAMS\FullServer.eu - Launcher\Odinstaluj.lnk"
	RMDir "$SMPROGRAMS\FullServer.eu - Launcher"
SectionEnd