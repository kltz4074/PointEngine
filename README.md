# PointEngine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-17-blue)](https://www.oracle.com/java/technologies/javase/jdk17-archive-downloads.html)
[![Gradle](https://img.shields.io/badge/Gradle-8.1.1-brightgreen)](https://gradle.org/)

**PointEngine** is a lightweight, open-source 3D engine written in Java, designed for creating games and visualizations.
It currently supports basic rendering and shader functionality, making it easy to experiment with 3D graphics.

---

## Features

| Feature               | Description                                                  |
| --------------------- | ------------------------------------------------------------ |
| Lightweight 3D engine | Simple and easy-to-use engine for Java projects              |
| Shader support        | Vertex and fragment shaders supported                        |
| Rendering             | Currently renders 3D objects like a cube                     |
| Build options         | Build into `.exe` or optionally `.jar`                       |
| Cross-platform        | Potentially cross-platform; currently tested on Windows only |
| Python scripting      | Use Python to script game logic and behaviors                |

---

## Installation

### Requirements

* **Java JDK 17**
* **Gradle 8.1.1**
* Windows (for now; Linux/Mac support will be tested later)

### Build

#### build crossplatform jar on **linux**: (tested on archlinux)
you need to be in root of the project

```bash
./gradlew shadowJar
```
example run command:

```bash
java -jar build/libs/PointEngine-1.0.0.jar
```

#### how to buld for windows:
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

| Feature               | Description                                                             |
| --------------------- | ----------------------------------------------------------------------- |
| Expand functionality  | Full game development support                                           |
| More shader support   | Advanced rendering techniques                                           |
| Cross-platform        | Improve support for Linux and Mac                                       |
| Documentation         | Tutorials and guides for developers                                     |
| **PointEngineEditor** | 3D editor for scenes and objects                                        |
| **Python scripting**  | Full Python integration for scripting game logic and behavior           |
| **PointHub**          | Central hub application for managing your projects and engine versions: |

* Install different versions of PointEngine
* Create new projects easily
* Open and manage existing projects

---

## License

The engine is fully open-source and licensed under the MIT License.
It must remain free to use. Games created with PointEngine can be either free or sold at the creator's discretion.

---

## Contributing

Contributions are welcome! If you want to help improve PointEngine, feel free to submit issues or pull requests.
