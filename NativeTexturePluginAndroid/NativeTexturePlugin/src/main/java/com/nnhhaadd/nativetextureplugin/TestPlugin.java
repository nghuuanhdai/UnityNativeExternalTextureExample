package com.nnhhaadd.nativetextureplugin;

public class TestPlugin {
    public interface OnInitializedListener{
        void onInitialized(int textureId);
    }
    private static UnityRenderTexture renderTexture;
    private static RandomCircleCanvas randomCircleCanvas;
    public static void TestRenderingTexture(int width, int height, OnInitializedListener listener){
        renderTexture = new UnityRenderTexture(width, height, surface->{
            randomCircleCanvas = new RandomCircleCanvas(surface, width, height);
            listener.onInitialized(renderTexture.getId());
        });
    }
}
