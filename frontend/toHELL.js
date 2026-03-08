fetch("http://localhost:8080/ping")
.then(res => {
  if (!res.ok) {
    // Исправлено: нужны обратные кавычки `, а не обычные '
    throw new Error(`HTTP error! status: ${res.status}`);
  }
  return res.json();
})
.then(data => {
  console.log('Success:', data);
  document.body.insertAdjacentHTML('beforeend', `<div>Ответ от сервера: ${data.text}</div>`);
})
.catch(err => {
  console.error('Error:', err);
  document.body.insertAdjacentHTML('beforeend', `<div style="color: red">Ошибка: ${err.message}</div>`);
});