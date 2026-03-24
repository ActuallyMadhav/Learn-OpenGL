# Build Instructions

## Step 0.5: Clean up build directory if it exists
Navigate to your project root in the terminal and run:
```bash
rm -rf build
```
## Step 1: Create the Build Directory
Navigate to your project root in the terminal and run:
```bash
mkdir build
cd build
```

## Step 2: Configure with CMake
This step generates the build files. On Apple Silicon (M1/M2/M3), use the prefix path hint:
```bash
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew ..
```

## Step 3: Compile the Code
```bash
make
```

## Step 4: Run the Application
```bash
./app
```