package org.PointEngine;

import org.joml.Matrix4f;
import org.joml.Vector3d;
import org.lwjgl.BufferUtils;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.MemoryStack;

import java.lang.reflect.Array;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;
import java.util.ArrayList;
import java.util.List;
import org.PointEngine.GameObject;

public class Main {

    Shader s;

    private long window;
    public float Size = 0.33f;
    public GameObject Player;
    List<GameObject> objects = new ArrayList<>();

    float[] vertices = {
            // position XYZ + color RGB
            -0.5f, -0.5f,  0.5f,  1f, 0f, 0f,
            0.5f, -0.5f,  0.5f,  1f, 0f, 0f,
            0.5f,  0.5f,  0.5f,  1f, 0f, 0f,
            -0.5f,  0.5f,  0.5f,  1f, 0f, 0f,
            -0.5f, -0.5f, -0.5f,  0f, 1f, 0f,
            0.5f, -0.5f, -0.5f,  0f, 1f, 0f,
            0.5f,  0.5f, -0.5f,  0f, 1f, 0f,
            -0.5f,  0.5f, -0.5f,  0f, 1f, 0f
    };

    int[] indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0
    };

    public static void main(String[] args) {
        new Main().run();
    }

    public void run() {
        System.out.println("Hello PointEngine 1.0.1!");
        init();
        loop();

        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void init() {
        GLFWErrorCallback.createPrint(System.err).set();

        if (!glfwInit())
            throw new IllegalStateException("Unable to initialize GLFW");

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(800, 600, "PointEngine", NULL, NULL);
        if (window == NULL)
            throw new RuntimeException("Failed to create the GLFW window");

        try (MemoryStack stack = stackPush()) {
            IntBuffer pWidth = stack.mallocInt(1);
            IntBuffer pHeight = stack.mallocInt(1);
            glfwGetWindowSize(window, pWidth, pHeight);

            GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowPos(window,
                    (vidmode.width() - pWidth.get(0)) / 2,
                    (vidmode.height() - pHeight.get(0)) / 2
            );
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwShowWindow(window);

        // Колбэк изменения размера окна
        glfwSetFramebufferSizeCallback(window, (win, width, height) -> {
            glViewport(0, 0, width, height);
        });
    }

    private void processInput(float devSpeed, float ScaleSpeed) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Player.z -= devSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Player.z += devSpeed;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Player.x -= devSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Player.x += devSpeed;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) Player.y += devSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) Player.y -= devSpeed;

        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) Size += ScaleSpeed;
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) Size -= ScaleSpeed;

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) Player.rotation.add(0.5, 0, 0);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) Player.rotation.add(-0.5, 0, 0);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) Player.rotation.add(0, -0.5, 0);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) Player.rotation.add(0, 0.5, 0);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    private void loop() {
        GL.createCapabilities();

        String vertCode =
                "#version 330 core\n" +
                        "layout (location = 0) in vec3 aPos;\n" +
                        "layout (location = 1) in vec3 aColor;\n" +
                        "out vec3 vertexColor;\n" +
                        "uniform mat4 projection;\n" +
                        "uniform mat4 model;\n" +
                        "void main() {\n" +
                        "    gl_Position = projection * model * vec4(aPos, 1.0);\n" +
                        "    vertexColor = aColor;\n" +
                        "}";

        String fragCode =
                "#version 330 core\n" +
                        "in vec3 vertexColor;\n" +
                        "out vec4 FragColor;\n" +
                        "void main() {\n" +
                        "    FragColor = vec4(vertexColor, 1.0);\n" +
                        "}";

        s = new Shader(vertCode, fragCode);

        int vao = GL30.glGenVertexArrays();
        GL30.glBindVertexArray(vao);

        int vbo = GL15.glGenBuffers();
        GL15.glBindBuffer(GL15.GL_ARRAY_BUFFER, vbo);
        GL15.glBufferData(GL15.GL_ARRAY_BUFFER, vertices, GL15.GL_STATIC_DRAW);

        int ebo = GL15.glGenBuffers();
        GL15.glBindBuffer(GL15.GL_ELEMENT_ARRAY_BUFFER, ebo);
        GL15.glBufferData(GL15.GL_ELEMENT_ARRAY_BUFFER, indices, GL15.GL_STATIC_DRAW);

        int stride = 6 * Float.BYTES;
        GL20.glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, 0);
        GL20.glEnableVertexAttribArray(0);
        GL20.glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, 3L * Float.BYTES);
        GL20.glEnableVertexAttribArray(1);

        GL30.glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0f, 0f, 0f, 1f);

        objects.add(new GameObject(0f, 0f, -5f, 0.5f, new Vector3d()));
        objects.add(new GameObject(0f, 1f, -5f, 0.5f, new Vector3d()));
        Player = new GameObject(0f, -1f, -5f, 0.5f, new Vector3d());
        objects.add(Player);

        while (!glfwWindowShouldClose(window)) {

            processInput(0.02f, 0.01f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            s.use();

            IntBuffer width = BufferUtils.createIntBuffer(1);
            IntBuffer height = BufferUtils.createIntBuffer(1);
            glfwGetFramebufferSize(window, width, height);

            // updating glViewport
            glViewport(0, 0, width.get(0), height.get(0));

            // recalculate matrix projection
            Matrix4f projection = new Matrix4f().perspective(
                    (float)Math.toRadians(45.0),
                    (float)width.get(0) / height.get(0),
                    0.1f, 100f
            );
            FloatBuffer projectionBuf = BufferUtils.createFloatBuffer(16);
            projection.get(projectionBuf);

            s.setMatrix4f("projection", projectionBuf);

            for (GameObject obj : objects) {
                obj.render(s, vao, indices.length);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        s.delete();
        GL30.glDeleteVertexArrays(vao);
        GL15.glDeleteBuffers(vbo);
        GL15.glDeleteBuffers(ebo);
    }
}
