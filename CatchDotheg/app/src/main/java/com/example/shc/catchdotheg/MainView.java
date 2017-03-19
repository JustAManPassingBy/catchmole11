package com.example.shc.catchdotheg;

import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.content.Context;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.os.*;
import android.content.res.Resources;



public class MainView extends SurfaceView implements SurfaceHolder.Callback{
    private MainActivity mainActivity;
    MainThread mainThread;
    private Handler handler;
    Context mainContext;
    ScreenConfig myScreen;
    boolean drawCls = false;
    private Resources RLib;
    Thread subThread = new Thread();

    float pushX = -1;
    float pushY = -1;

    // 두더지 4종류 0 = 빔, 1 = 일반, 2 = 황금, 3 = 방해꾼
    private Bitmap[] dothegs = new Bitmap[4];

    int life = 100;
    int score = 0;
    int catchs = 0;
    int timesec = 0;

    double GetRandom;
    int selected;
    int isfake;

    // 두더지 hole
    DtgButton[][] Holes = new DtgButton[3][3];


    public MainView(Context r , AttributeSet a) {
        super(r, a);

        getHolder().addCallback(this);

        mainThread = new MainThread(getHolder(), this);
        setFocusable(true);
        mainContext = r;

        handler = new Handler() {
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    default :
                        break;
                }
            }
        };

    }



    public void init (int width, int height, MainActivity mainactivity) {
        this.mainActivity = mainactivity;

        // 초기 화면 크기를 1000, 2000으로 설정
        myScreen = new ScreenConfig(width, height);
        myScreen.setSize(1000, 2000);

        // 두더지 버튼 사이즈
        int BtnWSize = (int)myScreen.getX(250);
        int BtnHSize = (int)myScreen.getY(400);

        // 리소스 적용
        RLib = getResources();

        // 두더지 이미지 할당
        dothegs[0] = BitmapFactory.decodeResource(RLib, R.drawable.empty);
        dothegs[1] = BitmapFactory.decodeResource(RLib, R.drawable.dotheg);
        dothegs[2] = BitmapFactory.decodeResource(RLib, R.drawable.gold);
        dothegs[3] = BitmapFactory.decodeResource(RLib, R.drawable.fake);

        // 두더지 이미지 크기 조정
        dothegs[0] = Bitmap.createScaledBitmap(dothegs[0], BtnWSize, BtnHSize, false);
        dothegs[1] = Bitmap.createScaledBitmap(dothegs[1], BtnWSize, BtnHSize, false);
        dothegs[2] = Bitmap.createScaledBitmap(dothegs[2], BtnWSize, BtnHSize, false);
        dothegs[3] = Bitmap.createScaledBitmap(dothegs[3], BtnWSize, BtnHSize, false);

        drawCls = true;

        // 두더지 만들기
        for(int i = 0 ; i < 9 ; i++) {
            int temp_x = i % 3;
            int temp_y = i / 3;

            Holes[temp_x][temp_y] = new DtgButton(i, BtnWSize, BtnHSize,
                    (int)myScreen.getX(125 + 250 * temp_x), (int)myScreen.getY(700 + 400 * temp_y));
            Holes[temp_x][temp_y].setState(0);

        }

        // 게임 시작
        gameStart();

    }

    @Override
    public void onDraw (Canvas canvas) {
        if (!drawCls) return;

        for (int i = 0 ; i < 9 ; i++) {
            int TempY =  i / 3;
            int TempX =  i % 3;

            Holes[TempX][TempY].draw(canvas, dothegs[Holes[TempX][TempY].State]);
        }


        canvas.drawColor(Color.rgb(255, 255, 255));

        // 점수 표시
        Paint backPaint = new Paint();
        backPaint.setColor(Color.rgb(0, 255, 0));
        canvas.drawText("Score : " + score, myScreen.getX(100) , myScreen.getY(300), backPaint);

        // 체력 표시
        // 피가 적을 땐 붉은 색으로
        if (life < 20) backPaint.setColor(Color.rgb(255,0,0));
        else backPaint.setColor(Color.rgb(0,0,255));

        canvas.drawText("Life : " + life + "%", myScreen.getX(600), myScreen.getY(300), backPaint);


        // 게임 오버 표시
        if (life <= 0) {
            backPaint.setColor(Color.rgb(255,0,0));
            backPaint.setTextSize(24);
            canvas.drawText("Game Over", myScreen.getX(400), myScreen.getY(900), backPaint);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            pushX = event.getX();
            pushY = event.getY();
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            PushAction(pushX, pushY);
            pushX = pushY = -1;
        }

        return true;
    }

    public void PushAction(float X, float Y) {
        for (int i = 0 ; i < 9 ; i++) {
            int TempY =  i / 3;
            int TempX =  i % 3;

            if (Holes[TempX][TempY].isSelected((int)X, (int)Y)) {
                Holes[TempX][TempY].Push(score , life, catchs);
            }
        }
    }

    // 타이머 조작
    public void gameStart () {
        // Life가 0가 아닐 때 까지
        while (life >= 0) {
            // 1000ms 대기
            try {
                subThread.sleep(1000);
            } catch (Exception ex) { }



            life = life - 5;
            timesec++;

            // 랜덤으로 두더지 한마리 출몰하도록
            GetRandom = java.lang.Math.random();
            selected = (int)(GetRandom * 9);
            isfake = (int)(GetRandom * 43);

            // 이미 두더지가 할당되 있을 경우 초기화
            if(Holes[selected % 3][selected / 3].State > 0)
                Holes[selected % 3][selected / 3].setState(0);

            // 황금 두더지 출몰, 매 7마리마다
            else if ((catchs > 0) && (catchs % 7 == 0))
                Holes[selected % 3][selected / 3].setState(2);

            // 페이크 두더지 출몰, isfake 값이 3 미만
            else if (isfake < 3)
                Holes[selected % 3][selected / 3].setState(3);

            // 이외의 경우 평범한 두더지 출몰
            else
                Holes[selected % 3][selected / 3].setState(1);
        }

    }



    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    public void surfaceCreated(SurfaceHolder holder) {
        mainThread.setRunning(true);

        try {
            if (mainThread.getState() == Thread.State.TERMINATED) {
                mainThread = new MainThread(getHolder(), this);
                mainThread.setRunning(true);
                setFocusable(true);
            }

            mainThread.start();
        } catch (Exception ex) {
            return;
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        boolean retry = true;
        mainThread.setRunning(false);

        while (retry) {
            try {
                mainThread.join();
                retry = false;
            } catch (Exception e) {
                return;
            }

        }
    }
}

