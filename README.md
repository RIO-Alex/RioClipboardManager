# 🚀 RioClipboardManager

![Platform](https://img.shields.io/badge/platform-Windows-blue?style=for-the-badge&logo=windows)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange?style=for-the-badge&logo=c%2B%2B)
![Framework](https://img.shields.io/badge/framework-Qt6-green?style=for-the-badge&logo=qt)
![License](https://img.shields.io/badge/license-MIT-red?style=for-the-badge)

**RioClipboardManager** - это мой простой и легкий менеджер буфера обмена, разработанный для **Windows**. Он автоматически сохраняет всё, что вы копируете, предоставляя мгновенный доступ к истории через горячие клавиши. Вы можете самостоятельно настроить размер истории, автозапуск, а также приложения, история копирования из которых не будет сохраняться.

---

## 📸 Скриншоты

| Главное меню | Удаление элемента | Настройки |
| :---: | :---: | :---: |
| ![Main Window](https://github.com/RIO-Alex/RioClipboardManager/blob/master/Screenshots/Screenshot_1.png?raw=true) | ![Main Window with delete](https://github.com/RIO-Alex/RioClipboardManager/blob/master/Screenshots/Screenshot_3.png?raw=true) | ![Settings](https://github.com/RIO-Alex/RioClipboardManager/blob/master/Screenshots/Screenshot_2.png?raw=true) |

---

## ✨ Основные функции
* **Глобальные хоткеи:** Вызывайте окно истории поверх любых приложений. По умолчанию - **ALT+C**.
* **Простое копирование и поиск:** Возвращайте в буфер обмена то, что уже копировали ранее одним кликом.
* **Хранение данных:** Все данные хранятся в локальной базе **SQLite**. Размер истории можно настроить.
* **Игнорируемые приложения:** Выбирайте сами, из каких приложений история копирования не будет сохраняться.
* **Простой UI:** Простой темный интерфейс, который не режет глаза ночью.
* **Автозагрузка:** Опциональный запуск вместе с Windows.

## 🛠 Технический стек
* **C++17**
* **Qt 6.9.1**
* **CMake**
* **Inno Setup**

## 📥 Как установить (Для Windows)
1. Перейдите в раздел [Releases](https://github.com/RIO-Alex/RioClipboardManager/releases).
2. Скачайте последний файл `RCM_Setup.exe`.
3. Запустите установку. 
   > ⚠️ **Важно:** Приложению требуются права администратора для работы глобальных горячих клавиш.

## 🔨 Сборка из исходников
Если вы разработчик и хотите модифицировать проект:
1. Установите **Qt Creator** и **Qt 6.x**.
2. Клонируйте проект: `git clone https://github.com/RIO-Alex/RioClipboardManager.git`
3. Откройте `CMakeLists.txt` и соберите проект через **MinGW**.

## 📄 Лицензия
Распространяется по лицензии **MIT**. Свободно для личного и коммерческого использования.

---
*Developed by RIO. 2026*