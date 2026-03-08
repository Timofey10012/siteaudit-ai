// Функция для отправки ping
async function pingServer() {
    const url = 'http://your-server.com:8080/api/ping'; // Адрес вашего бэкенда

    try {
        const response = await fetch(url, {
            method: 'GET', // Или 'POST', в зависимости от вашего endpoint'а
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (response.ok) {
            const data = await response.json();
            console.log('Ping успешен:', data);
            // Здесь можно обновить UI, например, зажечь зеленую лампочку
            document.getElementById('status').innerText = 'Сервер доступен';
        } else {
            console.error('Ошибка HTTP:', response.status);
            document.getElementById('status').innerText = 'Ошибка соединения';
        }
    } catch (error) {
        // Сработает при сетевой ошибке (сервер недоступен, CORS-проблемы и т.д.)
        console.error('Сетевая ошибка или сервер не отвечает:', error);
        document.getElementById('status').innerText = 'Сервер недоступен';
    }
}

// Вызвать функцию можно по таймеру или при загрузке страницы
setInterval(pingServer, 30000); // Пинговать каждые 30 секунд
// pingServer(); // Или один раз при загрузке