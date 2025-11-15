# GitHub Copilot / AI Agent Instructions

These instructions make an AI assistant immediately productive in this repository. Keep responses concise and follow existing patterns.

---
## 1. Project Scope & Architecture
- Single Arduino demo: `led_blink` sketches an UNO R4 MINIMA driving a 20x4 I2C LCD and a loop counter.
- Code split into `led_blink.ino` (implementation) and `led_blink.h` (prototypes + includes). Arduino preprocessor concatenates all `.ino` files in a folder; avoid adding multiple concern-mixed `.ino` files—prefer new directories for new demos.
- Documentation lives beside code: `led_blink/README.md`, `Detailed_Design.md`, rich HTML (`Detailed_Design.html`), and MISRA report (`MISRA_C_Check.html`). Update both Markdown + HTML when changing functional behavior.

## 2. Build & Deploy Workflow
Use Arduino CLI (preferred for automation):
```powershell
arduino-cli compile --fqbn arduino:renesas_uno:minima led_blink.ino
arduino-cli upload -p COM3 --fqbn arduino:renesas_uno:minima led_blink.ino
```
- Board FQBN: `arduino:renesas_uno:minima`.
- Typical compile time ~1.8s; upload ~15–18s (dfu-util).
- If LCD not detected, run (add back if removed) an I2C scanner sketch in a separate folder to avoid merge of multiple `.ino` in one build.

## 3. Coding Conventions (Derived From Existing Files)
- Braces: Allman style. One space between function name and `(`: `void setup ( void )`.
- Indent: 2 spaces; no tabs.
- Internal linkage constants: `static const` with explicit fixed-width types (`uint8_t`, `uint16_t`, `uint32_t`).
- Variables: `camelCase`; Constants: `UPPER_SNAKE_CASE`.
- Magic numbers avoided: define symbolic constants near top of `.ino`.
- MISRA rules referenced: 2.3, 2.5, 8.1, 8.2, 8.4. Maintain prototypes (`void loop ( void );`).
- Clear trailing LCD characters by overwriting with spaces (pattern: `lcd.print ( "        " );`). Preserve this to prevent residual digits when numbers shrink.

## 4. LCD Interaction Pattern
Sequence in `setup()`:
1. `lcd.init();`
2. `lcd.backlight();`
3. `lcd.clear();`
4. Fixed boot lines written sequentially using `lcd.setCursor(row,col)` then `lcd.print()`.
Update cycle in `loop()`: cursor → static label → dynamic value → blank padding → delay.
When adding new lines: keep within 20 columns, avoid un-cleared leftovers by padding spaces.

## 5. Extending Functionality
- Add new demo: create new folder (e.g., `sensor_demo/`) with its own `.ino` & `.h` rather than adding a second `.ino` under `led_blink/`.
- Introduce sensors: define new constants at top; keep LCD update atomic (setCursor + full line rewrite).
- Performance / frequency adjustments: change delay from `delay ( 1000 );`; if >1 line updated each second, group writes to minimize flicker.

## 6. Documentation Updates
When altering behavior (e.g., changing displayed text or timing):
- Sync `led_blink/README.md` (spec section) and `Detailed_Design.md` (setup, timing tables, memory stats if changed).
- If new constants: update the constants table in `Detailed_Design.md`.
- Keep HTML (`Detailed_Design.html`) SVG functional diagrams aligned (function tree, LCD layout). Add new labels with same color scheme (#2563eb background, white text).

## 7. Memory & Resource Awareness
- Current footprint (reference): flash ~45KB (17%), RAM globals ~5KB (15%). When adding features keep flash <50% unless necessary.
- Prefer lightweight arithmetic; avoid floating point unless sensor requires it.

## 8. Common Pitfalls & Guardrails
- Do NOT add multiple `.ino` files to same folder (Arduino build merges them unpredictably).
- Ensure I2C address constant matches physical module (`0x27` or `0x3F`). If changed, reflect in all docs.
- Avoid removing space padding after numeric prints; otherwise stale characters remain.
- Keep `setup()` and `loop()` signatures exactly: `void setup ( void )`, `void loop ( void )`.

## 9. Suggested AI Response Style
- Provide minimal diffs (focus only on changed regions) using existing formatting (align end-of-line comments at column ~80 when practical).
- When creating new constants or functions, mirror spacing style: `lcd.print     ( "Text" );` (aligned parentheses and columns) if expanding inside current file; otherwise may simplify but keep overall readability.

## 10. Next Steps For Improvements (If Asked)
- Optional modularization: extract LCD helper (e.g., `displayLine(uint8_t row, const char* text)` in a new `.h/.cpp` pair).
- Provide an I2C scanner under `tools/i2c_scanner/` for reuse.
- Add timing measurement using `millis()` around update blocks if performance tuning requested.

---
Feedback: Clarify any section to refine; request examples to expand patterns.
