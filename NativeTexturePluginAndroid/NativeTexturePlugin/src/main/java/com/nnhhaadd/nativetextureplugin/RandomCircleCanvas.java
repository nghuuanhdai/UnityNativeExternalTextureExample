package com.nnhhaadd.nativetextureplugin;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.view.Surface;

import java.util.Random;

public class RandomCircleCanvas {
    private final Surface targetSurface;
    private final int width, height;

    private final Rect rec;
    private final Paint p;
    private final Random rnd;
    private final Handler hnd;

    public RandomCircleCanvas(Surface targetSurface, int width, int height){
        this.targetSurface = targetSurface;
        this.width = width;
        this.height = height;

        hnd = new Handler(Looper.getMainLooper());
        rec = new Rect(0,0,width,height);
        p = new Paint();
        rnd = new Random();

        drawRandomCirclesInSurface();
    }

    private void drawRandomCirclesInSurface() {
        if(targetSurface != null)
        {
            Canvas c = targetSurface.lockCanvas(rec);
            p.setColor(Color.argb( 255, rnd.nextInt(255),rnd.nextInt(255),rnd.nextInt(255)));
            int radius = rnd.nextInt(100);
            c.drawCircle(rnd.nextInt(width),rnd.nextInt(height),radius,p);
            targetSurface.unlockCanvasAndPost(c);
        }

        hnd.postDelayed(new Runnable() {
            @Override
            public void run() {
                drawRandomCirclesInSurface();
            }
        },100);
    }

    public Surface getTargetSurface() {
        return targetSurface;
    }

    public int getHeight() {
        return height;
    }

    public int getWidth() {
        return width;
    }
}
