

-🧱 Entity Component System (ECS)
-🎮 Real-time rendering pipeline
-🎯 Mouse picking (ray casting)
-📦 OBJ model loading
-🎨 Custom shader system
-📷 Camera system (FPS-style movement)
-🖥️ ImGui-based editor UI
-⚙️ Basic physics system (in progress)# lets create the engine mann

#🏗️ Engine Architecture

The engine is built with a modular structure:

-Renderer → Handles drawing and OpenGL calls
-Scene System → Manages entities and components
-Mesh / Material → Rendering abstraction
-Camera → View & projection control
-PhysicsSystem → Updates simulation
-UI (ImGui) → Editor tools and debugging

#🎮 Controls
-W / A / S / D → Move camera
-Mouse Click → Select object (ray picking)
-UI Panel → Scene interaction

#🚀 Getting Started
Requirements
-C++17+
-OpenGL
-GLFW
-GLAD
-GLM

#📂 Project Structure
/Engine
  ├── Renderer
  ├── Physics
  ├── UI
  └── Utils
  └──Resources 

#🔮 Future Improvements
-Lighting system (Phong / PBR)
-Shadow mapping
-Better physics integration
-Scene serialization (save/load)
-GPU optimizations

#👨‍💻 Author

Built by a mechanical engineering student exploring graphics, simulation, and engine development.