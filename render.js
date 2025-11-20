const puppeteer = require('puppeteer');
const path = require('path');

(async () => {
  try {
    const browser = await puppeteer.launch({
      headless: true,
      args: [
        '--no-sandbox',
        '--disable-setuid-sandbox',
        '--enable-accelerated-2d-canvas',
        '--enable-gpu-rasterization',
        '--use-gl=desktop',
        '--enable-webgl',
        '--disable-software-rasterizer'
      ]
    });

    const page = await browser.newPage();
    const file = 'file://' + path.resolve('RH850_F1KMS1_Startup/Detailed_Design.html');
    await page.goto(file, { waitUntil: 'networkidle2' });

    // Adjust viewport and scale for higher-quality export
    const width = 1600;
    const height = 2000;
    await page.setViewport({ width, height, deviceScaleFactor: 2 });

    // Wait a little in case SVG scripts/styles need time
    await page.waitForTimeout(500);

    const out = 'detailed_design.png';
    await page.screenshot({ path: out, fullPage: true });
    console.log('Saved', out);
    await browser.close();
  } catch (err) {
    console.error('Render failed:', err);
    process.exit(1);
  }
})();
