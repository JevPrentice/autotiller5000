# AutoTiller5000 Test Runner for Windows PowerShell
# Requires MinGW-w64 (g++) in PATH - install via MSYS2: https://www.msys2.org/

Write-Host "AutoTiller5000 Test Suite" -ForegroundColor Cyan
Write-Host "========================" -ForegroundColor Cyan

# Check if g++ is available
try {
    $null = Get-Command g++ -ErrorAction Stop
    $version = & g++ --version 2>$null | Select-Object -First 1
    Write-Host "Found: $version" -ForegroundColor Green
} catch {
    Write-Host "ERROR: g++ not found in PATH" -ForegroundColor Red
    Write-Host "Please install MinGW-w64 via MSYS2 and add to PATH" -ForegroundColor Yellow
    Write-Host "Download: https://www.msys2.org/" -ForegroundColor Yellow
    Write-Host "After install, add C:\msys64\ucrt64\bin to PATH" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

# Compiler settings
$CXX = "g++"
$CXXFLAGS = "-Wall", "-std=c++11"
$LDFLAGS = "-lm"

# Test files
$TESTS = @(
    "test_navigation",
    "test_safety_logic",
    "test_button_logic",
    "test_tack_logic",
    "test_led_patterns",
    "test_course_adjustment",
    "test_state_machine",
    "test_servo_control",
    "test_sensitivity_adjustment",
    "test_settings_persistence",
    "test_watchdog_safety"
)

Write-Host "`nCompiling tests..." -ForegroundColor Yellow
Write-Host ""

# Compile each test
$compileErrors = @()
foreach ($test in $TESTS) {
    Write-Host "Compiling $test..." -NoNewline

    $args = $CXXFLAGS + @("-o", "$test.exe", "$test.cpp") + $LDFLAGS
    $process = Start-Process -FilePath $CXX -ArgumentList $args -Wait -PassThru -NoNewWindow

    if ($process.ExitCode -eq 0) {
        Write-Host " OK" -ForegroundColor Green
    } else {
        Write-Host " FAILED" -ForegroundColor Red
        $compileErrors += $test
    }
}

if ($compileErrors.Count -gt 0) {
    Write-Host "`nCompilation errors in:" -ForegroundColor Red
    $compileErrors | ForEach-Object { Write-Host "  $_" -ForegroundColor Red }
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host "`nRunning tests..." -ForegroundColor Yellow
Write-Host "================" -ForegroundColor Yellow

# Run each test
$testResults = @()
foreach ($test in $TESTS) {
    Write-Host "`nRunning $test" -ForegroundColor Cyan
    Write-Host ("-" * 40) -ForegroundColor Gray

    $process = Start-Process -FilePath ".\$test.exe" -Wait -PassThru -NoNewWindow

    if ($process.ExitCode -eq 0) {
        Write-Host "✓ $test PASSED" -ForegroundColor Green
        $testResults += @{Name=$test; Status="PASSED"}
    } else {
        Write-Host "✗ $test FAILED" -ForegroundColor Red
        $testResults += @{Name=$test; Status="FAILED"}
    }
}

# Summary
Write-Host "`n================" -ForegroundColor Yellow
Write-Host "Test Summary:" -ForegroundColor Yellow
Write-Host "================" -ForegroundColor Yellow

$passed = ($testResults | Where-Object {$_.Status -eq "PASSED"}).Count
$failed = ($testResults | Where-Object {$_.Status -eq "FAILED"}).Count

foreach ($result in $testResults) {
    if ($result.Status -eq "PASSED") {
        Write-Host "✓ $($result.Name)" -ForegroundColor Green
    } else {
        Write-Host "✗ $($result.Name)" -ForegroundColor Red
    }
}

Write-Host "`nResults: $passed passed, $failed failed" -ForegroundColor $(if ($failed -eq 0) {"Green"} else {"Red"})

# Clean up executables
Write-Host "`nCleaning up..." -ForegroundColor Yellow
foreach ($test in $TESTS) {
    if (Test-Path "$test.exe") {
        Remove-Item "$test.exe" -Force
    }
}

if ($failed -eq 0) {
    Write-Host "All tests completed successfully! 🎉" -ForegroundColor Green
} else {
    Write-Host "$failed test(s) failed!" -ForegroundColor Red
    exit 1
}

Write-Host "`nDone." -ForegroundColor Cyan
Read-Host "Press Enter to exit"