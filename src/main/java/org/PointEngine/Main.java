package org.PointEngine;

import org.joml.Matrix4f;
import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;

import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL15.*;
import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Main {

    Shader s;

    private long window;
    private float Speed = 0.05f;
    private float posX = 0.0f, posY = 0.0f, posZ = -5.0f;
    private float angle = 0.0f;
    public float Size = 0.33f;

    float[] vertices = {
            // Позиция XYZ + Цвет RGB
            -0.5f, -0.5f,  0.5f,  1f, 0f, 0f, // перед
            0.5f, -0.5f,  0.5f,  1f, 0f, 0f,
            0.5f,  0.5f,  0.5f,  1f, 0f, 0f,
            -0.5f,  0.5f,  0.5f,  1f, 0f, 0f,

            -0.5f, -0.5f, -0.5f,  0f, 1f, 0f, // зад
            0.5f, -0.5f, -0.5f,  0f, 1f, 0f,
            0.5f,  0.5f, -0.5f,  0f, 1f, 0f,
            -0.5f,  0.5f, -0.5f,  0f, 1f, 0f
    };

    int[] indices = {
            0, 1, 2, 2, 3, 0,       // перед
            4, 5, 6, 6, 7, 4,       // зад
            0, 4, 7, 7, 3, 0,       // лево
            1, 5, 6, 6, 2, 1,       // право
            3, 2, 6, 6, 7, 3,       // верх
            0, 1, 5, 5, 4, 0        // низ
    };

    public static void main(String[] args) {
        new Main().run();
    }

    public void run() {
        System.out.println("Hello PointEngine " + "1.0.1 " + "!");
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
    }

    private void processInput(float devSpeed, float ScaleSpeed) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) posZ -= devSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) posY += devSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) posX -= devSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) posX += devSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) posY -= devSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) posZ += devSpeed;

        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) Size += ScaleSpeed;
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) Size -= ScaleSpeed;

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
                        "}\n";

        String fragCode =
                "#version 330 core\n" +
                        "in vec3 vertexColor;\n" +
                        "out vec4 FragColor;\n" +
                        "void main() {\n" +
                        "    FragColor = vec4(vertexColor, 1.0);\n" +
                        "}";

        s = new Shader(vertCode, fragCode);

        int vao = glGenVertexArrays();
        glBindVertexArray(vao);

        int vbo = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

        int ebo = glGenBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        int stride = 6 * Float.BYTES;
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, 3L * Float.BYTES);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0f, 0f, 0f, 1f);

        IntBuffer width = BufferUtils.createIntBuffer(1);
        IntBuffer height = BufferUtils.createIntBuffer(1);
        glfwGetFramebufferSize(window, width, height);
        Matrix4f projection = new Matrix4f().perspective(
                (float) Math.toRadians(45.0),
                (float) width.get(0) / height.get(0),
                0.1f, 100f
        );

        FloatBuffer projectionBuf = BufferUtils.createFloatBuffer(16);
        projection.get(projectionBuf);

        while (!glfwWindowShouldClose(window)) {

            processInput(0.05f, 0.01f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            s.use();

            Matrix4f model = new Matrix4f()
                    .translate(posX, posY, posZ)
                    .rotateY((float) Math.toRadians(angle))
                    .scale(Size);

            FloatBuffer modelBuf = BufferUtils.createFloatBuffer(16);
            model.get(modelBuf);

            s.setMatrix4f("projection", projectionBuf);
            s.setMatrix4f("model", modelBuf);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indices.length, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            angle += 0.5f;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        s.delete();
        glDeleteVertexArrays(vao);
        glDeleteBuffers(vbo);
        glDeleteBuffers(ebo);
    }
}
