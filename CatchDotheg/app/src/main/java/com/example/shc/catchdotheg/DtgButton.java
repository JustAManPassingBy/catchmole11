package com.example.shc.catchdotheg;

import android.graphics.*;

public class DtgButton {
    public int ButtonNum;
    public int ButtonX;
    public int ButtonY;
    private int ButtonWidth;
    private int ButtonHeight;
    public int State = 0;
    public boolean isPush = false;
    public boolean drawable = true;


    // 초기화
    public DtgButton(int btnnum, int width, int height, int X, int Y) {
        ButtonNum = btnnum;
        ButtonWidth = width;
        ButtonHeight = height;
        ButtonX = X;
        ButtonY = Y;
    }

    // 현재 상태  변경
    public void setState(int curstate) {
        State = curstate;
    }

            // 눌렸을 때 현재 상태에 따라 이벤트 결정
        public void Push(int Score, int Life, int amount_catch) {
            isPush = true;
            // Dotheg Empty 상태
            if (State == 0) {
                Life = Life - 5;
                amount_catch = 0;
            }

            // 1마리의 DotheG
            else if (State == 1) {
                Life = Life + 5;
                Score = Score + 100;
                amount_catch++;
            }

            // Golden dotheg
            else if (State == 2) {
                Life = Life + 5;
                Score = Score + 500;
            amount_catch++;
        }

        // Trap
        else if (State == 3) {
            Life = Life - 10;
            if (Score >= 100) {
                Score = Score - 100;
            } else {
                Score = 0;
            }
            amount_catch = 0;
        }

        State = 0;
        isPush = false;
    }

    public boolean isSelected (int screenX, int screenY) {
        if ((screenX >= ButtonX && screenX <= ButtonX + ButtonWidth) &&
                (screenY >= ButtonY && screenY <= ButtonY + ButtonHeight)) {
            return true;
        }

        return false;
    }

    public void draw(Canvas canvas, Bitmap bitmap) {
        if (drawable) {
            canvas.drawBitmap(bitmap, ButtonX, ButtonY, null);
        }
    }
}
