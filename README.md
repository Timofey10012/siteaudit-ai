# SiteAudit-AI

> Полноценный README в процессе подготовки

Автоматизированный аудит UX сайтов с использованием AI.

## Команда

- **Тимофей** ([@Timofey10012](https://github.com/Timofey10012)) — backend, архитектура, сборка, scraper, интеграция (C++, CMake, vcpkg)
- **Андрей** ([@loominator911](https://github.com/loominator911)) — frontend (HTML/CSS/JS)

## Быстрый старт

```bash
git clone --recursive https://github.com/Timofey10012/siteaudit-ai.git
```

### Установить переменные окружения (глобально).
Пример строки подключения есть в .env.example.

Примечание: для сборки рекомендуется использовать x64 Native Tools Command Prompt for Visual Studio (MSVC toolchain).

```
cd SiteAudit-AI
```
```
cmake --preset default
```
```
cmake --build --preset default
```

Собранный .exe находится в папке build/.

Запуск:
```
./build/ai_ux_auditor_backend.exe
```

### Фронт не требует сборки. Просто откройте файл frontend/welcomToTheHELL.html в браузере.
