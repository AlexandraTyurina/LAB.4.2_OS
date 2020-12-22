#include <iostream>
#include <wchar.h>
#include <windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;
    HANDLE pipe = NULL;
    const int BufferSize = 512;
    std::wstring lpPipeName = L"\\\\.\\pipe\\NamedPipel4";
    OVERLAPPED overlapped;

    do
    {
        std::cout << "1 - Подключиться к каналу" << std::endl;
        std::cout << "2 - Прочитать данные" << std::endl;
        std::cout << "3 - Отсоединиться от канала" << std::endl;
        std::cout << "0 - Выход" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            pipe = CreateFile(lpPipeName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
            if (pipe == INVALID_HANDLE_VALUE)
                std::cout << "Ошибка при подключении к каналу" << GetLastError() << std::endl;
            else
                std::cout << "Подключение к каналу произведено успешно" << std::endl;
            break;
        case 2:
            overlapped.Offset = 0;
            overlapped.OffsetHigh = 0;
            char message[BufferSize];
            ReadFileEx(pipe, message, BufferSize, &overlapped, NULL);
            std::cout << "Сообщение: " << message << std::endl;
            break;
        case 3:
            std::cout << "Отключения соединения" << std::endl;
            CloseHandle(pipe);
            break;
        case 0:
            break;
        }
    } while (choice != 0);

    return 0;
}
