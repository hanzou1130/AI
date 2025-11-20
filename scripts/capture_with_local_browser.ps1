$file = "file:///" + (Join-Path (Get-Location) "RH850_F1KMS1_Startup/Detailed_Design.html")
$out = Join-Path (Get-Location) 'detailed_design_browser.png'

$candidates = @(
    @{ name='msedge'; paths=@('C:\Program Files\Microsoft\Edge\Application\msedge.exe','C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe') ; args='--headless=new --disable-software-rasterizer --use-gl=desktop --enable-gpu-rasterization --window-size=1600,2000 --screenshot="{out}" "{file}"' },
    @{ name='chrome'; paths=@('C:\Program Files\Google\Chrome\Application\chrome.exe','C:\Program Files (x86)\Google\Chrome\Application\chrome.exe') ; args='--headless --disable-gpu --screenshot="{out}" --window-size=1600,2000 "{file}"' },
    @{ name='brave'; paths=@('C:\Program Files\BraveSoftware\Brave-Browser\Application\brave.exe','C:\Program Files (x86)\BraveSoftware\Brave-Browser\Application\brave.exe') ; args='--headless --disable-gpu --screenshot="{out}" --window-size=1600,2000 "{file}"' },
    @{ name='chrome-path'; paths=@((Get-Command chrome -ErrorAction SilentlyContinue).Source) ; args='--headless --disable-gpu --screenshot="{out}" --window-size=1600,2000 "{file}"' },
    @{ name='firefox'; paths=@('C:\Program Files\Mozilla Firefox\firefox.exe','C:\Program Files (x86)\Mozilla Firefox\firefox.exe') ; args='--headless --screenshot "{out}" --width 1600 --height 2000 "{file}"' }
)

$found = $false
foreach ($c in $candidates) {
    foreach ($p in $c.paths) {
        if (-not $p) { continue }
        if (Test-Path $p) {
            Write-Host "Found $($c.name) at $p"
            $argstr = $c.args -replace '\{out\}', ($out -replace '\\','/') -replace '\{file\}', ($file -replace '\\','/')
            Write-Host "Running: $p $argstr"
            $proc = Start-Process -FilePath $p -ArgumentList $argstr -Wait -PassThru
            if ($proc.ExitCode -eq 0) {
                Write-Host "Screenshot saved to $out"
                $found = $true
                break
            } else {
                Write-Host "Process exited with code $($proc.ExitCode)"
            }
        }
    }
    if ($found) { break }
}

if (-not $found) {
    Write-Host "No supported browser found or screenshot failed. You can install Node.js + Puppeteer or Inkscape as alternative." -ForegroundColor Yellow
    exit 2
}
exit 0
