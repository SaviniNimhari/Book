$ErrorActionPreference = 'Stop'

$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $projectRoot

$sourceFiles = @(
    (Join-Path $projectRoot 'src\main.cpp')
    (Join-Path $projectRoot 'src\Library.cpp')
)

$outputPath = Join-Path $projectRoot 'library-management.exe'

if (Get-Command g++ -ErrorAction SilentlyContinue) {
    g++ -std=c++17 $sourceFiles -o $outputPath
}
elseif (Get-Command clang++ -ErrorAction SilentlyContinue) {
    clang++ -std=c++17 $sourceFiles -o $outputPath
}
elseif (Get-Command cl.exe -ErrorAction SilentlyContinue) {
    & cl.exe /std:c++17 /EHsc /Fe:$outputPath $sourceFiles
}
else {
    throw 'No C++ compiler found. Install MinGW g++, clang++, or Visual Studio Build Tools first.'
}

Write-Host 'Starting backend...'
& $outputPath