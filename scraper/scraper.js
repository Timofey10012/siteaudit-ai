const { chromium } = require("playwright");

async function autoScroll(page) {
    await page.evaluate(async () => {

        await new Promise((resolve) => {

            let totalHeight = 0;
            const distance = 500;

            const timer = setInterval(() => {

                window.scrollBy(0, distance);
                totalHeight += distance;

                if (totalHeight >= document.body.scrollHeight) {
                    clearInterval(timer);
                    resolve();
                }

            }, 200);

        });

    });
}

async function scrape() {

    const url = process.argv[2];

    if (!url) {
        console.error("URL not provided");
        process.exit(1);
    }

    const browser = await chromium.launch({
        headless: true
    });

    const context = await browser.newContext({
        userAgent: "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/120 Safari/537.36"
    });

    const page = await context.newPage();
    
    try {

        await page.goto(url, {
            waitUntil: "networkidle",
            timeout: 30000
        });

        await autoScroll(page);

        const result = await page.evaluate(() => {

            function cleanText(text) {
                return text
                    .replace(/\s+/g, " ")
                    .trim();
            }

            const title = document.title;

            let mainContent =
                document.querySelector("article") ||
                document.querySelector("main");

            if (!mainContent) {

                const blocks = Array.from(document.querySelectorAll("div"));

                let maxText = 0;

                blocks.forEach(el => {

                    const len = el.innerText.length;

                    if (len > maxText) {
                        maxText = len;
                        mainContent = el;
                    }

                });

            }

            const headings = Array.from(
                document.querySelectorAll("h1, h2, h3")
            ).map(e => cleanText(e.innerText));

            const paragraphs = Array.from(
                mainContent.querySelectorAll("p")
            )
                .map(e => cleanText(e.innerText))
                .filter(t => t.length > 40);

            const links = Array.from(
                mainContent.querySelectorAll("a")
            )
                .map(e => e.href)
                .filter(l => l.startsWith("http"));

            return {
                title,
                headings,
                paragraphs,
                links
            };

        });

        console.log(JSON.stringify(result));

    } catch (err) {

        console.error("Scraping error:", err.message);
        process.exit(1);

    } finally {
        await browser.close();
    }
}

scrape();