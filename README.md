# Клиент-Серверное приложение с MySQL 🖥️🔌📊

**Проект включает клиент для отправки запросов и сервер с сохранением в базу данных**

[![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B)](https://isocpp.org/)
[![MySQL](https://img.shields.io/badge/MySQL-8.0+-blue?logo=mysql)](https://www.mysql.com/)

## 📌 Описание проекта

Проект состоит из двух компонентов:
1. **Клиент** - отправляет сообщения серверу через командную строку
2. **Сервер** - принимает сообщения и сохраняет их в MySQL базу данных

## 🚀 Быстрый старт

### Требования
- Компилятор C++17
- MySQL Server 8.0+
- Библиотеки:
  - `libmysqlcppconn-dev` (для работы с MySQL)
  - `libmysqlclient-dev`

### Установка зависимостей (Ubuntu)
bash
sudo apt update
sudo apt install g++ libmysqlcppconn-dev libmysqlclient-dev

### Сборка и запуск
```
bash
# Клонировать репозиторий
git clone https://github.com/Giffulf/ClientServerProject.git
cd ClientServerProject

# Сборка клиента
g++ -std=c++17 client.cpp -o client -lmysqlcppconn

# Сборка сервера
g++ -std=c++17 server.cpp -o server -lmysqlcppconn

# Запуск сервера (в отдельном терминале)
./server

# Запуск клиента (в другом терминале)
./client "Ваше сообщение"
```

## 🛠 Функционал

### Клиентская часть
- Отправка сообщений серверу через командную строку
- Обработка аргументов командной строки
- Получение подтверждения от сервера

### Серверная часть
- Прослушивание порта 8080
- Прием сообщений от клиентов
- Сохранение сообщений в MySQL базу
- Отправка подтверждения клиенту

## ⚙️ Настройка базы данных

1. Создайте базу данных и таблицу:
```
sql
CREATE DATABASE testdb;
USE testdb;

CREATE TABLE requests (
    id INT AUTO_INCREMENT PRIMARY KEY,
    message TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

2. Настройте пользователя MySQL (в server.cpp):
```
cpp
// Измените параметры подключения
sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
std::unique_ptr<sql::Connection> con(driver->connect(
    "tcp://127.0.0.1:3306", 
    "ваш_пользователь", 
    "ваш_пароль"
));
```

## 💻 Пример работы

1. Запустите сервер:
```
./server
Сервер запущен и слушает порт 8080
```
3. Отправьте сообщение через клиент:
```
./client "Тестовое сообщение"
Сообщение отправлено: Тестовое сообщение
Ответ сервера: Данные успешно записаны в базу
```
5. Проверьте запись в MySQL:
```
sql
SELECT * FROM testdb.requests;
```
