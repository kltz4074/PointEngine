package org.PointEngine;

import org.joml.Matrix4f;
import org.joml.Vector3d;
import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL30;
import java.nio.FloatBuffer;

import static org.lwjgl.opengl.GL11.*;

public class GameObject {
    public float x, y, z;
    public float scale;
    public Vector3d rotation;


    public GameObject(float x, float y, float z, float scale, Vector3d rotation) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.scale = scale;
        this.rotation = rotation;
    }

    public void render(Shader s, int vao, int indexCount) {
        Matrix4f model = new Matrix4f()
                .translate(x, y, z)
                .rotateY((float) Math.toRadians(rotation.y))
                .rotateZ((float) Math.toRadians(rotation.z))
                .rotateX((float) Math.toRadians(rotation.x))
                .scale(scale);

        FloatBuffer modelBuf = BufferUtils.createFloatBuffer(16);
        model.get(modelBuf);
        s.setMatrix4f("model", modelBuf);

        GL30.glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        GL30.glBindVertexArray(0);
    }
}
