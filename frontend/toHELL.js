async function pingBackend() {
  try {
    const start = Date.now();
    
    const response = await fetch('https://your-backend.com/api/ping', {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
      },
    });
    
    const end = Date.now();
    const latency = end - start;
    
    if (response.ok) {
      const data = await response.json();
      console.log('Ping успешен!', data);
      console.log(`Задержка: ${latency}ms`);
      return { success: true, latency, data };
    } else {
      throw new Error('Сервер вернул ошибку');
    }
  } catch (error) {
    console.error('Ping не удался:', error);
    return { success: false, error: error.message };
  }
}

// Использование
pingBackend().then(result => {
  if (result.success) {
    console.log(`Сервер отвечает за ${result.latency}ms`);
  }
});