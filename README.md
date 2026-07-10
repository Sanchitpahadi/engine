# 🚀 Physics Simulation Engine (C++ / OpenGL)

## 📌 Overview

This project is a custom-built simulation engine developed from scratch using C++ and OpenGL. It began as an exploration of rendering systems and low-level graphics, and has evolved into a framework for physics-based simulation.

The goal is to move beyond visual rendering and toward **physically meaningful simulation**, focusing on force-based motion and gravitational systems.
And will grow more towards a perfect engine for simulation coders.
---

## 🎯 Current Features

* Custom rendering pipeline using OpenGL
* Entity-component style architecture
* Transform system (position, rotation, scale)
* Camera system (view + projection)
* Basic physics system:

  * Mass, velocity, acceleration
  * Force accumulation
  * Gravity (inverse-square law)
* Real-time simulation loop
* ImGui-based debugging UI

---

## 🧠 Motivation

Most beginner projects focus only on rendering. This project is built to explore:

* How physical systems are modeled computationally
* How simulation stability and accuracy affect results
* How to transition from graphics to engineering-oriented simulations

---

## ⚙️ Architecture Overview

The engine is structured into modular systems:

* **Scene** → manages entities and components
* **Transform Component** → spatial data
* **Renderer** → mesh + shader pipeline
* **Physics System** → motion and force updates
* **Camera** → view/projection handling
* **UI Layer (ImGui)** → debugging and interaction

---

## 🔬 Physics Model

The simulation is based on Newtonian mechanics:

**Force:**
F = m × a

**Gravity:**
F = G × (m₁ × m₂) / r²

**Integration:**

* Velocity updated from acceleration
* Position updated from velocity

> Note: This is an early-stage implementation and is being improved for numerical stability.

---

## 🚧 Current Limitations

* Basic integration method (not stable for long-term simulation)
* Uses float precision (needs double for large-scale systems)
* No advanced numerical methods yet (e.g., RK4)
* No real-world validation yet

---

## 🚀 Future Work

* Stable orbital simulation
* Improved integration techniques
* Double precision support
* Multi-body gravitational systems
* Extension toward engineering simulations
* Research-oriented development

---

## 🖥️ Build & Run

```bash
git clone https://github.com/Sanchitpahadi/engine
cd engine

mkdir build
cd build

cmake ..
make
```

---

## 📊 Vision

This project aims to become a platform for:

* Physics simulation experimentation
* Bridging graphics and engineering
* Learning computational modeling from first principles

---

## 🤝 Feedback

This project is actively being developed.
Suggestions, feedback, and discussions are welcome.

---

## 👤 Author

Sanchit Pahadi
