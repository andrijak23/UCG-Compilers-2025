@echo off
echo === Pokrecem Bison...
bison -d parser.y

echo === Pokrecem Flex...
flex scanner.l

echo === Kompajliram sa gcc...
gcc -o kompajler main.c parser.tab.c lex.yy.c ast.c

if errorlevel 1 (
    echo === Greska prilikom kompajlacije!
    pause
    exit /b
)

echo === Gotovo!
echo.

set /p startRun="Da li zelis da odmah pokrenes kompajler.exe (test.txt)? [y/n]: "
if /i "%startRun%"=="y" (
    echo === Pokrecem...
    kompajler test.txt
) else (
    echo === Kompajler nije pokrenut.
)

pause
