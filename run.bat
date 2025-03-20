@echo off

:: Compilar el proyecto
mkdir build
cd build
cmake ..
cmake --build .

:: Ejecutar el servidor en segundo plano
start Server.exe

:: Esperar 2 segundos para asegurarse de que el servidor esté listo
timeout /t 2

:: Ejecutar el cliente
Client.exe

:: Detener el servidor después de que el cliente termine
taskkill /im Server.exe /f