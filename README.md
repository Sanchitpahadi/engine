# Simp

> I really dont know how long will it take but gonna make a engine that gonna help in make simulations of rockets 

---

## ✨ Features

- 🧱 Entity Component System (ECS)
- 🎮 Real-time rendering pipeline
- 🎯 Mouse picking (ray casting)
- 📦 OBJ model loading
- 🎨 Custom shader system
- 📷 FPS-style camera
- 🖥️ ImGui editor UI
- ⚙️ Basic physics system (WIP)

---

## 🏗️ Architecture

### 🔹 Core Systems

| System        | Description |
|--------------|------------|
| Renderer     | Handles OpenGL drawing |
| Scene        | Manages entities & components |
| Mesh/Material| Rendering abstraction |
| Camera       | View & projection control |
| Physics      | Simulation updates |
| UI (ImGui)   | Debug tools & editor |

---

## 🎮 Controls

- **W / A / S / D** → Move camera  
- **Mouse Click** → Select object (ray picking)  
- **UI Panel** → Scene interaction  

---

## 🛠️ Tech Stack

- C++17+
- OpenGL
- GLFW
- GLAD
- GLM
- ImGui

---

## 📂 Project Structure
Engine/
├── Renderer/
├── Physics/
├── UI/
├── Utils/
└── Resources/


---


## 🚧 Future Improvements

- 💡 Lighting (Phong / PBR)
- 🌑 Shadow mapping
- ⚙️ Better physics
- 💾 Scene save/load
- 🚀 GPU optimizations

---

## 👨‍💻 Author

Built by a mechanical engineering student exploring:
- Graphics
- Simulation
- Engine development