#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <сообщение>\n";
        return 1;
    }

    const char* message = argv[1];

    // Создаем сокет
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    // Адрес сервера (локальный хост)
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    
    // Преобразуем IP-адрес из текста в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    // Отправка сообщения
    if (send(sock, message, strlen(message), 0) == -1) {
        perror("send");
        close(sock);
        return 1;
    }

    std::cout << "Сообщение отправлено: " << message << std::endl;

    // Получение ответа от сервера
    char buffer[1024] = {0};
    int valread = read(sock, buffer, sizeof(buffer));
    if (valread > 0) {
        std::cout << "Ответ сервера: " << std::string(buffer, valread) << std::endl;
    }

    close(sock);
    return 0;
}