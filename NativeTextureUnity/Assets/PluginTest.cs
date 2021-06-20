using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.UI;

public class PluginTest : MonoBehaviour
{
    [DllImport ("NativeUnityRenderingCallback")]
    private static extern IntPtr GetRenderEventFunc ();

    [SerializeField]
    private RawImage rawImage;

    private class PluginTestCallback : AndroidJavaProxy
    {
        private Action<int> callback;
        public PluginTestCallback(Action<int> callback) : base("com.nnhhaadd.nativetextureplugin.TestPlugin$OnInitializedListener")
        {
            this.callback = callback;
        }
        private void onInitialized(int textureId){
            this.callback(textureId);
        }
    }

    private IntPtr nativeTexPtr;
    private bool nativeTexPtrSet;

    void Start()
    {
        CommandBuffer commandBuffer = new CommandBuffer();
        commandBuffer.IssuePluginEvent(GetRenderEventFunc(), 1);
        Camera.main.AddCommandBuffer(CameraEvent.BeforeSkybox, commandBuffer);

        new AndroidJavaClass("com.nnhhaadd.nativetextureplugin.TestPlugin")
            .CallStatic(
                "TestRenderingTexture",
                512,
                512,
                new PluginTestCallback(texId=>{
                    nativeTexPtr = (IntPtr)texId;
                    nativeTexPtrSet = true;
                })
            );
        StartCoroutine(WaitForNativePointer());
    }

    private IEnumerator WaitForNativePointer()
    {
        yield return new WaitUntil(()=>nativeTexPtrSet);
        Texture2D texture2D = Texture2D.CreateExternalTexture(512,512, TextureFormat.RGBA32, false, true, nativeTexPtr);
        Debug.Log($"texture created: ${nativeTexPtr}");
        rawImage.texture = texture2D;
    }
}
