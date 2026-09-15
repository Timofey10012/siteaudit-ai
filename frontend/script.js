const btn = document.querySelector('button');
const input = document.querySelector('.inp');
const resultBlock = document.querySelector('.second_block div');
const secdiv = document.querySelector(".second_block");


btn.addEventListener('click', async () => {
  const url = input.value.trim();

  if (!url) {
    resultBlock.innerHTML = '<p style="color: orange; font-weight: bold;"> Вставь хоть что-нибудь!</p>';
    return;
  }

  if (!URL.canParse(url)) {
    resultBlock.innerHTML = 
      '<p style="color: #e74c3c; font-weight: bold;"> Это не похоже на нормальный URL<br>' +
      'Пример: https://example.com</p>';
    return;
  }

  resultBlock.innerHTML = '<p style="color: #27ae60;">URL выглядит корректно! Начинаю анализ...</p>';


try {
    const response = await fetch('http://localhost:8080/analyze', {
        method: 'POST',         
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ url: url }),
    });

    if (!response.ok) {
        throw new Error(`Сервер ответил ошибочкой: ${response.status}`);
    }

    const data = await response.json();

    console.log(data);

    // Парсим analyze (если это строка)
    const parsed = typeof data.analyze === 'string' 
        ? JSON.parse(data.analyze) 
        : data.analyze;

    // Собираем проблемы
    let issuesHtml = '';
    if (parsed.issues && parsed.issues.length > 0) {
        issuesHtml = '<ul list-style-type: none;>';
        parsed.issues.forEach(issue => {
            issuesHtml += `
                <li>
                    <strong>${issue.title}</strong> 
                    <span style="color: ${issue.severity === 'high' ? '#e74c3c' : issue.severity === 'medium' ? '#f39c12' : '#27ae60'}">
                        (${issue.severity})
                    </span>
                </li>
            `;
        });
        issuesHtml += '</ul>';
    } 
    else {
        issuesHtml = '<p>Нет проблем</p>';
    }

    // Собираем улучшения
    let improvementsHtml = '';
    if (parsed.improvements && parsed.improvements.length > 0) {
        improvementsHtml = '<ul>';
        parsed.improvements.forEach(imp => {
            improvementsHtml += `<li>${imp}</li>`;
        });
        improvementsHtml += '</ul>';
    } 
    else {
        improvementsHtml = '<p>Нет советов</p>';
    }

    // Собираем сильные стороны
    let strengthsHtml = '';
    if (parsed.strengths && parsed.strengths.length > 0) {
        strengthsHtml = '<ul>';
        parsed.strengths.forEach(str => {
            strengthsHtml += `<li>${str}</li>`;
        });
        strengthsHtml += '</ul>';
    } 
    else {
        strengthsHtml = '<p>Нет сильных сторон</p>';
    }

    // Выводим результат
    resultBlock.innerHTML = `
        <p><strong>📄 Тип страницы:</strong> ${parsed.pageType}</p>
        <p><strong>⭐ Общая оценка сайта:</strong> ${parsed.overallScore}</p>
        <p><strong>📝 Короткое описание:</strong> ${parsed.shortDescription}</p>
        
        <p><strong>🔍 Основные проблемы:</strong></p>
        ${issuesHtml}
        
        <p><strong>💡 Советы по улучшению:</strong></p>
        ${improvementsHtml}
        
        <p><strong>✨ Сильные стороны UX дизайна:</strong></p>
        ${strengthsHtml}
    `;

} 
catch (err) {
    console.error(err);
    resultBlock.innerHTML = `<p style="color: #e74c3c; font-weight: bold;">❌ Ошибка: ${err.message}</p>`;
}});