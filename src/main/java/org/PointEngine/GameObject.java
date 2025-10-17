package org.PointEngine;

import org.joml.Matrix4f;
import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL30;
import java.nio.FloatBuffer;

import static org.lwjgl.opengl.GL11.*;

public class GameObject {
    public float x, y, z;
    public float scale;
    public float rotation;

    public GameObject(float x, float y, float z, float scale) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.scale = scale;
    }

    public void render(Shader s, int vao, int indexCount) {
        Matrix4f model = new Matrix4f()
                .translate(x, y, z)
                .rotateY((float)Math.toRadians(rotation))
                .scale(scale);

        FloatBuffer modelBuf = BufferUtils.createFloatBuffer(16);
        model.get(modelBuf);

        s.setMatrix4f("model", modelBuf);

        GL30.glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        GL30.glBindVertexArray(0);
    }
}
