# Running Tests on Windows

This guide shows how to run the AutoTiller5000 tests on Windows using the PowerShell script.

## One-Time Setup (10 minutes)

### Step 1: Download MSYS2
1. Go to: **https://www.msys2.org/**
2. Click the big **"Download"** button
3. Download the `.exe` file (about 85MB)

### Step 2: Install MSYS2
1. **Double-click** the downloaded file
2. Click **"Next"** through all the screens (accept all defaults)
3. When it says "Run MSYS2 now" - **check the box**
4. Click **"Finish"**

### Step 3: Update MSYS2
1. A black terminal window opens automatically
2. Type: `pacman -Syuu`
3. Press **Enter**
4. When it asks `Proceed with installation? [Y/n]` - type **Y** and press **Enter**
5. **The window might close itself - this is normal!**

### Step 4: Install the Compiler
1. **Open MSYS2** again (search "MSYS2" in Start menu)
2. Type: `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`
3. Press **Enter**
4. When it asks `Proceed with installation? [Y/n]` - type **Y** and press **Enter**
5. Wait for it to download and install (2-3 minutes)

### Step 5: Add to Windows PATH
1. Press **Windows key + R**
2. Type: `sysdm.cpl` and press **Enter**
3. Click **"Environment Variables"** button
4. In the bottom box, find **"Path"** and click **"Edit"**
5. Click **"New"**
6. Type: `C:\msys64\ucrt64\bin`
7. Click **"OK"** on all windows

### Step 6: Test It Works
1. Press **Windows key + R**
2. Type: `cmd` and press **Enter**
3. Type: `g++ --version`
4. You should see version information (not "command not found")

**That's it!** The setup is complete.

## Running Tests

### Method 1: Right-click (Easiest)
1. Navigate to the `test/` folder
2. Right-click `run_tests.ps1`
3. Select "Run with PowerShell"

### Method 2: PowerShell Command Line
1. Open PowerShell in the `test/` folder
2. Run: `.\run_tests.ps1`

### If Execution Policy Blocks Script
If you get an execution policy error, run this once:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

## What the Script Does

1. **Checks Dependencies**: Verifies g++ is installed and accessible
2. **Compiles Tests**: Builds all 11 test files with proper C++11 flags
3. **Runs Tests**: Executes each test and captures results
4. **Shows Summary**: Color-coded pass/fail results with counts
5. **Cleans Up**: Removes compiled .exe files automatically

## Troubleshooting

**"g++ not found"**: MinGW-w64 not in PATH. Reinstall or fix PATH.

**"Execution policy"**: Run `Set-ExecutionPolicy RemoteSigned -Scope CurrentUser`

**Tests fail**: Same as Unix - check code for actual bugs in the implementation.

## Output Example

```
AutoTiller5000 Test Suite
========================
Found: g++ (Rev10, Built by MSYS2 project) 13.2.0

Compiling tests...

Compiling test_navigation... OK
Compiling test_safety_logic... OK
...

Running tests...
================

Running test_navigation
----------------------------------------
✓ test_navigation PASSED

...

================
Test Summary:
================
✓ test_navigation
✓ test_safety_logic
...

Results: 11 passed, 0 failed
All tests completed successfully! 🎉
```