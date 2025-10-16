# PointEngine

**PointEngine** is a lightweight, open-source 3D engine written in Java, designed for creating games and visualizations.
Currently, it supports basic rendering and shader functionality, making it easy to experiment with 3D graphics.

---

## Features

* Lightweight 3D engine for Java
* Shader support
* Rendering of 3D objects (currently tested with a cube)
* Easy build into `.exe` or optionally `.jar`
* Cross-platform potential (currently tested on Windows only)

---

## Installation

### Requirements

* **Java JDK 17**
* **Gradle**
* Windows (for now; Linux/Mac support will be tested later)

### Build

You can build the engine and your game with a single command:

```bash
gradlew shadowJar launch4j
```

This will create an `.exe` file in the `build` folder.
Optionally, you can build a `.jar` file if needed.

---

## Usage

You can run the engine by executing the generated `.exe` file.

### Example

<!-- Insert your demo GIF here -->
![CubeAnim](https://github.com/user-attachments/assets/71a66f44-af3b-40b0-8943-3128d494d2a2)



Currently, the engine renders a simple 3D cube. More features and examples will be added in future updates.

---

## Future Plans

* Expand engine functionality for creating full games
* Add more shader support and rendering techniques
* Improve cross-platform support (Linux, Mac)
* Add documentation and tutorials for developers
* PointEngineEditor - 3D editor
* PointHub (like unityhub:)- app for :
  1) installing different versions of engine
  2) creating project
  3) openning projects
* python - coding language support
---

## License

The engine is fully open-source and must remain free to use. Games created with PointEngine can be either free or sold at the creator's discretion.

---

## Contributing

Contributions are welcome! If you want to help improve PointEngine, feel free to submit issues or pull requests.
