// scraper.js
const { chromium } = require('playwright');

const url = process.argv[2];

if (!url) {
    console.log(JSON.stringify({
        status: "error",
        message: "Не передан URL"
    }));
    process.exit(1);
}

(async () => {
    const browser = await chromium.launch({ 
        headless: true,
        args: [
            '--disable-blink-features=AutomationControlled',
            '--no-sandbox'
        ]
    });
    
    const page = await browser.newPage();
    
    await page.setExtraHTTPHeaders({
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/120.0.0.0 Safari/537.36'
    });
    
    await page.setViewportSize({ width: 1920, height: 1080 });
    
    await page.addInitScript(() => {
        Object.defineProperty(navigator, 'webdriver', {
            get: () => false,
        });
    });

    try {
        let response;
        try {
            response = await page.goto(url, { 
                waitUntil: 'networkidle',
                timeout: 15000 
            });
        } catch (networkIdleError) {
            response = await page.goto(url, { 
                waitUntil: 'domcontentloaded',
                timeout: 15000 
            });
        }

        if (response && response.status() >= 400) {
            throw new Error(`HTTP ${response.status()}: ${response.statusText() || "Страница не найдена"}`);
        }

        const isSPA = await page.evaluate(() => {
            return !!document.querySelector('#app, #root, [data-react-root], [data-vue-app], ng-app');
        });
        
        if (isSPA) {
            await page.waitForTimeout(3000);
        }

        await autoScrollWithInfinite(page);

        await page.waitForTimeout(1000);

        const structuredData = await page.evaluate(() => {
            const data = {
                title: document.title || "",
                headings: [],
                textContent: [],
                interactiveElements: new Map(),
                forms: []
            };

            function getText(el) {
                return el?.textContent?.trim()?.replace(/\s+/g, ' ') || "";
            }

            function traverse(node) {
                if (!node || node.nodeType !== Node.ELEMENT_NODE) return;
                
                const tag = node.tagName.toLowerCase();

                if (/^h[1-6]$/.test(tag)) {
                    const text = getText(node);
                    if (text) data.headings.push({ level: tag, text });
                }
                
                else if (tag === 'p') {
                    const text = getText(node);
                    if (text && text.length > 20) {
                        data.textContent.push(text);
                    }
                }
                
                if (tag === 'a' || tag === 'button') {
                    const text = getText(node);
                    const href = node.href || "";
                    if (text && href !== '#' && !href.includes('javascript:')) {
                        const key = text + href;
                        if (!data.interactiveElements.has(key)) {
                            data.interactiveElements.set(key, { 
                                text: text, 
                                href: href,
                                type: tag
                            });
                        }
                    }
                }
                
                if (tag === 'form') {
                    const formData = {
                        action: node.action || "",
                        method: node.method || "get",
                        fields: [],
                        submitButton: null,
                        hasAgreement: false
                    };
                    
                    node.querySelectorAll('input, textarea, select').forEach(field => {
                        const fieldType = field.type || field.tagName.toLowerCase();
                        const fieldName = field.name || "";
                        const placeholder = field.placeholder || "";
                        const required = field.required || field.hasAttribute('required');
                        
                        if (fieldType === 'hidden') return;
                        
                        formData.fields.push({
                            type: fieldType,
                            name: fieldName,
                            placeholder: placeholder,
                            required: required
                        });
                    });
                    
                    const submitBtn = node.querySelector('button[type="submit"], input[type="submit"], button:not([type="button"])');
                    if (submitBtn) {
                        formData.submitButton = getText(submitBtn) || "Отправить";
                    }
                    
                    const agreementCheckbox = node.querySelector('input[type="checkbox"]');
                    if (agreementCheckbox) {
                        const label = agreementCheckbox.closest('label')?.innerText || "";
                        const parentText = agreementCheckbox.parentElement?.innerText || "";
                        const fullText = (label + " " + parentText).toLowerCase();
                        if (fullText.includes('согласи') || fullText.includes('согласен') || fullText.includes('политик')) {
                            formData.hasAgreement = true;
                        }
                    }
                    
                    if (formData.fields.length > 0) {
                        data.forms.push(formData);
                    }
                }

                const children = node.children;
                if (children) {
                    for (let i = 0; i < children.length; i++) {
                        traverse(children[i]);
                    }
                };
            }

            traverse(document.body);
            
            return {
                title: data.title,
                headings: data.headings,
                textContent: data.textContent.join("\n"),
                interactiveElements: Array.from(data.interactiveElements.values()),
                forms: data.forms
            };
        });

        console.log(JSON.stringify({
            status: "ok",
            content: JSON.stringify(structuredData)
        }));

    } catch (err) {
        console.log(JSON.stringify({
            status: "error",
            message: err.message
        }));
    } finally {
        await browser.close();
    }

    async function autoScrollWithInfinite(page) {
        await page.evaluate(async () => {
            await new Promise((resolve) => {
                let lastHeight = 0;
                let attempts = 0;
                let scrollAttempts = 0;
                const maxScrollAttempts = 30;
                const distance = 400;
                
                const timer = setInterval(() => {
                    const scrollHeight = document.body.scrollHeight;
                    window.scrollBy(0, distance);
                    scrollAttempts++;
                    
                    if (scrollHeight === lastHeight) {
                        attempts++;
                        if (attempts >= 3) {
                            clearInterval(timer);
                            resolve();
                        }
                    } else {
                        attempts = 0;
                        lastHeight = scrollHeight;
                    }
                    
                    if (window.scrollY + window.innerHeight >= scrollHeight - 100) {
                        if (attempts >= 2) {
                            clearInterval(timer);
                            resolve();
                        }
                    }
                    
                    if (window.scrollY > 100000 || scrollAttempts >= maxScrollAttempts) {
                        clearInterval(timer);
                        resolve();
                    }
                }, 800);
            });
        });
    }
})();