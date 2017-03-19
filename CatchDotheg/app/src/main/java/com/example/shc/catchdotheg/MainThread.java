package com.example.shc.catchdotheg;

import android.view.SurfaceHolder;
import android.graphics.Canvas;
import android.util.*;

public class MainThread extends Thread {
    private SurfaceHolder surfaceHolder;
    private MainView mainView;
    private boolean running = false;

    double fpsStartTime = 0.0f;
    double fpsLastTime = 0.0f;
    // FPS 변화 주기(millisecond단위)
    double fpsPeriod = 200;

    public MainThread(SurfaceHolder surfaceholder, MainView mainView) {
        surfaceHolder = surfaceholder;
        this.mainView = mainView;
    }

    public SurfaceHolder getSurfaceHolder() {
        return surfaceHolder;
    }

    public void setRunning (boolean isrun) {
        running = isrun;
    }

    @Override
    public void run() {
        try
        {
            Canvas c;
            while (running) {
                c = null;

                try {
                    c = surfaceHolder.lockCanvas(null);
                    synchronized (surfaceHolder) {
                        try {
                            Thread.sleep(1);

                            // 20ms당 1번씩 출력하도록
                            fpsStartTime = System.currentTimeMillis();
                            if (fpsStartTime - fpsLastTime > fpsPeriod) {
                                mainView.onDraw(c);
                                fpsLastTime = fpsStartTime;
                            }
                        } catch (Exception exTemp) {
                            return;
                        }
                    }
                }
                finally {
                    if (c != null) surfaceHolder.unlockCanvasAndPost(c);
                }
            }
        } catch (Exception exTot) {
            return;
        }
    }
}
