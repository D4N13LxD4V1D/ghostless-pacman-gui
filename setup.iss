[Setup]
AppName=Ghostless Pacman
AppVersion=1.0
WizardStyle=modern
DefaultDirName={autopf}\Ghostless Pacman
DefaultGroupName=Ghostless Pacman
UninstallDisplayIcon={app}\Ghostless Pacman.exe
Compression=lzma2
SolidCompression=yes
OutputDir=release

ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "Ghostless Pacman.exe"; DestDir: "{app}"; DestName: "Ghostless Pacman.exe"
Source: "libpng16-16.dll"; DestDir: "{app}"
Source: "player.png"; DestDir: "{app}"    
Source: "SDL2.dll"; DestDir: "{app}"
Source: "SDL2_image.dll"; DestDir: "{app}"
Source: "zlib1.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Ghostless Pacman"; Filename: "{app}\Ghostless Pacman.exe"
