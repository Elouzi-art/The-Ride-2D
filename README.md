# 🚴‍♂️ The Ride – 2D Bike Racing Game (C++ / SDL2)

## 🎯 Overview

**The Ride** is a 2D bike racing game developed in **C++** using the **SDL2 (v2.30.1)** library.  
Players control a bike, avoid obstacles, and try to survive as long as possible.  
This project applies **object-oriented programming (OOP)** principles and explores graphics programming with SDL2 on **Windows**.

The project demonstrates key concepts such as modular design, rendering, event handling, and collision management.

---

## 🧱 Project Structure

The_Ride/
│
├── bin/ → Executable files (not versioned)  
├── include/ → Header files (.hpp)  
├── lib/ → SDL libraries (.a, .dll.a)  
├── res/ → Resources (images, fonts…)  
├── audio/ → Sound effects and music (.wav)  
├── src/ → Source files (.cpp)  
│   ├── main.cpp  
│   ├── menu.cpp  
│   └── … other gameplay files  
└── README.md

---

## 🧩 Object-Oriented Design

| Class                | Responsibility                                  |
| -------------------- | ----------------------------------------------- |
| **Game**             | Manages initialization, main loop, and shutdown |
| **Menu**             | Handles the main menu interface and navigation  |
| **Player (Bike)**    | Controls player position, movement, and gravity |
| **Map / Level**      | Manages obstacles and environment               |
| **Timer**            | Handles game timing and countdowns              |
| **CollisionManager** | Detects and processes collisions                |

---

## 🧠 Main Features

✅ Interactive graphical menu using **SDL2_ttf**  
✅ Smooth rendering with **SDL_Renderer**  
✅ Player movement with gravity simulation  
✅ Collision detection with obstacles  
✅ Modular and maintainable code structure  
✅ Audio effects and music using **SDL2_mixer**  
✅ Compatible with Windows (MinGW64)

---

## 🧰 Technologies

- **Language:** C++17  
- **Libraries:** SDL2, SDL2_ttf, SDL2_image, SDL2_mixer  
- **Tools:** MinGW64 (g++), Visual Studio Code  
- **Paradigm:** Object-Oriented Programming (OOP)

---

## 👨‍💻 Team

Developed by a **two-person team**:

- **Salmene Elouzi**  
  - Set up SDL2 and SDL_ttf libraries  
  - Created the project structure and initial interface  
  - Implemented player gravity and rendering logic  
  - Built the main menu system  

- **Marwane Chnitfa**  
  - Implemented gameplay logic and collision system  
  - Managed keyboard navigation and testing

---

## ⚙️ Build Instructions

### On **Windows (MinGW64)**:

1. Clone the repository:
git clone https://github.com/Elouzi-art/The-Ride-2D.git
2. cd The-Ride-2D
3. Compile using g++:
g++ src/*.cpp -o bin/The_ride -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

---

📜 License
This project is open-source under the MIT License.
See the LICENSE file for details.






