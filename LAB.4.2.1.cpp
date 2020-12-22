#include <iostream>
#include <windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;
    HANDLE pipe = NULL;
    const int BufferSize = 512;
    OVERLAPPED overlapped;
    overlapped.hEvent = CreateEvent(NULL, true, false, L"Event");
    HANDLE event = NULL;
    std::wstring lpPipeName = L"\\\\.\\pipe\\NamedPipel4";
    do
    {
        std::cout << "\t Выбрать действие: " << std::endl;
        std::cout << "1 - Создание канала" << std::endl;
        std::cout << "2 - Соединение с каналом" << std::endl;
        std::cout << "3 - Запись данных" << std::endl;
        std::cout << "4 - Отключения соединения с каналом" << std::endl;
        std::cout << "0 - Выход" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            //FILE_FLAG_OVERLAPPED - использование асинхронных операций,PIPE_TYPE_MESSAGE-записать как сообщение
            pipe = CreateNamedPipe(lpPipeName.c_str(), PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE, 1, BufferSize, BufferSize, 0, NULL);
            if (pipe == INVALID_HANDLE_VALUE)
                std::cout << " Ошибка в создании канала" << GetLastError() << std::endl;
            else
                std::cout << " Канал успешно создан" << std::endl;
            break;
        case 2:
        {
            BOOL fConnected = ConnectNamedPipe(pipe, NULL) ?
                TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            if (fConnected)
            {
                std::cout << " Приложение-клиент было успешно подключено" << std::endl;
            }
            else
                std::cout << " Ошибка подключения приложения-клиента" << std::endl;

        }
            break;
        case 3:
            overlapped.Offset = 0;
            overlapped.OffsetHigh = 0;

            char message[BufferSize];
            std::cout << " Введите сообщение: ";
            std::cin >> message;

            DWORD written;

            WriteFile(pipe, message, strlen(message) + 1, &written, &overlapped);
            if (!written)
                std::cout << " Ошибка" << GetLastError() << std::endl;
            else
            {
                std::cout << " Отправка сообщения" << std::endl;
                WaitForSingleObject(event, INFINITE);
                std::cout << " Сообщение доставлено" << std::endl;
            }
            break;
        case 4:
            std::cout << "Отключения соединения" << std::endl;
            DisconnectNamedPipe(pipe);
            CloseHandle(event);
            CloseHandle(pipe);
        case 0:
            break;
        }
    } while (choice != 0);
    if (pipe)
    {
        DisconnectNamedPipe(pipe);
        CloseHandle(event);
        CloseHandle(pipe);
    }
    return 0;
}
