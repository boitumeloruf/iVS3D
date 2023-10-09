@ECHO OFF
ECHO Downloading test resources...
SET "outpath=%~dp0"

if not exist "%outpath%/testresources" (
powershell.exe -Command "$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest -Uri 'https://drive.google.com/uc?export=download&id=1tR3vq25R-qqbgzOoyMUvKVnEPJm7UE6o' -OutFile %outpath%/test.zip; Expand-Archive %outpath%/test.zip -DestinationPath %outpath%/testresources; Remove-Item %outpath%/test.zip;"
)

ECHO Finished downloading...