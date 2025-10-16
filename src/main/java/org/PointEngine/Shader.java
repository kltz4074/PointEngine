package org.PointEngine;

import static org.lwjgl.opengl.GL20.*;
import static org.lwjgl.opengl.GL20C.glDeleteProgram;

public class Shader {
    private int programId;

    public Shader(String vertexCode, String fragmentCode) {
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, vertexCode);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, fragmentCode);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);
        checkLinkErrors(programId);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    public void use() {
        glUseProgram(programId);
    }

    public void setMatrix4f(String name, java.nio.FloatBuffer value) {
        int loc = glGetUniformLocation(programId, name);
        glUniformMatrix4fv(loc, false, value);
    }

    public void delete() {
        glDeleteProgram(programId);
    }

    private void checkCompileErrors(int shader, String type) {
        if (glGetShaderi(shader, GL_COMPILE_STATUS) == GL_FALSE) {
            System.err.println("ERROR::SHADER_COMPILATION_ERROR of type: " + type);
            System.err.println(glGetShaderInfoLog(shader));
        }
    }

    private void checkLinkErrors(int program) {
        if (glGetProgrami(program, GL_LINK_STATUS) == GL_FALSE) {
            System.err.println("ERROR::PROGRAM_LINKING_ERROR");
            System.err.println(glGetProgramInfoLog(program));
        }
    }
}
