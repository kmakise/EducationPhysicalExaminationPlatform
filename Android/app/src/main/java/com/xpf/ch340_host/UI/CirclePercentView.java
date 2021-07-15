package com.xpf.ch340_host.UI;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Shader;
import android.support.annotation.Keep;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

import com.xpf.ch340_host.R;


/**
 * 圆形进度条（可设置 线性渐变-背景色-进度条颜色-圆弧宽度）
 * Created by ZWQ
 */
public class CirclePercentView extends View {

    public static final int WIDTH_RADIUS_RATIO = 8;     // 弧线半径 : 弧线线宽 (比例)
    public static final int MAX = 100;
    private Paint mPaint;
    private float progressPercent;
    private int radius;//圆弧宽度
    private RectF rectF;
    private int bgColor, progressColor;
    private int startColor, endColor;
    private LinearGradient gradient;
    private boolean isGradient;

    public CirclePercentView(Context context) {
        super(context);
        init();
    }

    public CirclePercentView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.CirclePercentView);
        bgColor = typedArray.getColor(R.styleable.CirclePercentView_circleBgColor1, getResources().getColor(R.color.gray_cfcfcf));
        progressColor = typedArray.getColor(R.styleable.CirclePercentView_circleProgressColor1, getResources().getColor(R.color.orange_ffc032));
        radius = typedArray.getInt(R.styleable.CirclePercentView_circleRadius1, WIDTH_RADIUS_RATIO);
        isGradient = typedArray.getBoolean(R.styleable.CirclePercentView_circleIsGradient1, false);
        startColor = typedArray.getColor(R.styleable.CirclePercentView_circleStartColor1, getResources().getColor(R.color.black_3A3D4E));
        endColor = typedArray.getColor(R.styleable.CirclePercentView_circleEndColor1, getResources().getColor(R.color.black_475B80));
        typedArray.recycle();
        init();
    }

    public CirclePercentView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        setMeasuredDimension(getMeasuredWidth(), getMeasuredWidth());//自定义的View能够使用wrap_content或者是match_parent的属性
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        gradient = new LinearGradient(getWidth(), 0, getWidth(), getHeight(), startColor, endColor, Shader.TileMode.MIRROR);
    }

    @SuppressWarnings("Duplicates")
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        // 1、绘制背景灰色圆环
        int centerX = getWidth() / 2;
        int strokeWidth = centerX / radius;
        mPaint.setShader(null);//必须设置为null，否则背景也会加上渐变色
        mPaint.setStrokeWidth(strokeWidth); //设置画笔的大小
        mPaint.setColor(bgColor);
        canvas.drawCircle(centerX, centerX, centerX - strokeWidth / 2, mPaint);
        // 2、绘制比例弧
        if (rectF == null) {//外切正方形
            rectF = new RectF(strokeWidth / 2, strokeWidth / 2, 2 * centerX - strokeWidth / 2, 2 * centerX - strokeWidth / 2);
        }
        //3、是否绘制渐变色
        if (isGradient) {
            mPaint.setShader(gradient);//设置线性渐变
        } else {
            mPaint.setColor(progressColor);
        }
        canvas.drawArc(rectF, -90, 3.6f * progressPercent, false, mPaint);   //画比例圆弧
    }

    private void init() {
        mPaint = new Paint();
        //画笔样式
        mPaint.setStyle(Paint.Style.STROKE);
        //设置笔刷的样式:圆形
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        //设置抗锯齿
        mPaint.setAntiAlias(true);
    }

    @Keep
    public void setPercentage(float percentage) {
        this.progressPercent = percentage;
        invalidate();
    }

    public void setRadius(int radius) {
        this.radius = radius;
    }

    public void setBgColor(int bgColor) {
        this.bgColor = bgColor;
    }

    public void setProgressColor(int progressColor) {
        this.progressColor = progressColor;
    }

    public void setStartColor(int startColor) {
        this.startColor = startColor;
    }

    public void setEndColor(int endColor) {
        this.endColor = endColor;
    }

    public void setGradient(boolean gradient) {
        isGradient = gradient;
    }
}
