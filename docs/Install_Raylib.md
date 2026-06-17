# Raylib Setup Guide for Ubuntu

This is a quick-start guide to installing the Raylib library from source on Ubuntu Linux and compiling your first 3D application. (If you're not on Ubuntu, you need to lurn how to have friends, but good luck!)

---

## 1. Prerequisites & Installation

Open your terminal and run the following commands to install the required compiler tools, system graphics libraries, and clone/build Raylib.

```bash
# Update package lists (cause why not)
sudo apt update
```

```bash
# Install build tools and CMake if you dont already have it
sudo apt install build-essential git cmake
```

```bash
# Install required development graphic and audio dependencies to run raylib on Linux
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

```bash
# Clone the official Raylib repository (If link expired, you're on your own)
git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src
```

```bash
# Compile for desktop and install globally into your system
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

---

## 2. Testing the library

There is an example folder called 'examples' in the cloned repository, where you can find examples to try the library. 

```bash
cd examples
make core/core_3d_camera_first_person PLATFORM=PLATFORM_DESKTOP
./core/core_3d_camera_first_person
```

This should open a window with cube environment. I should be able to control the camera using the mouse.

---

## 3. How to use the library in your project

Create a file named main.c (or main.cpp if you prefer C++, but don't forget to encapsulate C function calls, cause we're good C++ devs). You simply include <raylib.h> at the top. To enter 3D mode, you initialize a Camera3D struct and wrap your 3D drawing functions inside BeginMode3D(camera) and EndMode3D().

Here is a minimal starting template for a 3D project:

```c
#include <raylib.h>

int main(void) {
    // 1. Initialize Window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "My First Raylib 3D Project");

    // 2. Define the 3D Camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Looking at the center
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation)
    camera.fovy = 45.0f;                                // Field of view
    camera.projection = CAMERA_PERSPECTIVE;             // Camera type

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update variables here (e.g., rotate camera, move objects)

        // Draw everything
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // --- Enter 3D Mode ---
            BeginMode3D(camera);
                
                // Draw a simple red cube at coordinates (0, 0, 0)
                DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 2.0f, 2.0f, 2.0f, RED);
                // Draw a grid on the floor so you can see perspective
                DrawGrid(10, 1.0f);

            EndMode3D();
            // --- Exit 3D Mode ---

            DrawText("Congrats! You are rendering in 3D!", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
```

After this, you compile like this:
```bash
gcc main.c -o my_3d_game -lraylib -lGL -lm -lpthread -lrt -lX11
```

---

Normally, if you follow this tutorial, it should work. If you have any issues, i guess it's just a skill issue :( .