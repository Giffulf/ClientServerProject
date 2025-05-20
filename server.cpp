#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

const int PORT = 8080;

void saveToDatabase(const std::string& message) {
    try {
        // Подключение к базе данных MySQL
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "root", "ваш_пароль"));

        con->setSchema("testdb");

        // Подготовленный запрос для вставки данных
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "INSERT INTO requests (message) VALUES (?)"
        ));
        pstmt->setString(1, message);
        pstmt->executeUpdate();

        std::cout << "Данные успешно записаны в базу.\n";
        
        // Можно отправить подтверждение клиенту внутри функции обработки соединения.
        
    } catch (sql::SQLException& e) {
        std::cerr << "Ошибка базы данных: " << e.what() << "\n";
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Создаем сокет файла для прослушивания входящих соединений
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // любой IP на машине
    address.sin_port = htons(PORT);

    // Связываем сокет с портом и IP
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать входящие соединения
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
std::cout << "Сервер запущен и слушает порт " << PORT << "\n";

while (true) {
     // Принимаем входящее соединение
     if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                              (socklen_t*)&addrlen))<0) {
         perror("accept");
         continue; // Продолжаем слушать дальше при ошибке accept()
     }
     
     // Читаем сообщение от клиента
     char buffer[1024] = {0};
     int valread = read(new_socket , buffer, sizeof(buffer));
     if (valread > 0) {
         std::string received_message(buffer, valread);
         std::cout << "Получено сообщение: " << received_message << "\n";

         // Записываем в базу данных
         saveToDatabase(received_message);

         // Отправляем подтверждение клиенту
         const char* reply = "Запрос получен и записан.";
         send(new_socket , reply , strlen(reply), 0 );
     }
     close(new_socket); // закрываем соединение после обработки одного клиента
}

close(server_fd);
return 0;
}