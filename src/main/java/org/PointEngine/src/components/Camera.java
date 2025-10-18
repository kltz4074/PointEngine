package org.PointEngine.src.components;

import org.joml.Matrix4f;
import org.joml.Vector3d;

public class Camera {
    public float x, y, z;
    public Vector3d rotation; // rotation.x = pitch, rotation.y = yaw

    public Camera(float x, float y, float z, Vector3d rotation) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.rotation = rotation;
    }

    public Matrix4f getViewMatrix() {
        // вычисляем направление взгляда из pitch и yaw
        Vector3d front = new Vector3d();
        front.x = Math.cos(Math.toRadians(rotation.x)) * Math.sin(Math.toRadians(rotation.y));
        front.y = Math.sin(Math.toRadians(rotation.x));
        front.z = -Math.cos(Math.toRadians(rotation.x)) * Math.cos(Math.toRadians(rotation.y));
        front.normalize();

        Vector3d center = new Vector3d(x, y, z).add(front);
        Vector3d up = new Vector3d(0, 1, 0);

        return new Matrix4f().lookAt(
                new org.joml.Vector3f((float)x, (float)y, (float)z),
                new org.joml.Vector3f((float)center.x, (float)center.y, (float)center.z),
                new org.joml.Vector3f((float)up.x, (float)up.y, (float)up.z)
        );
    }
}
