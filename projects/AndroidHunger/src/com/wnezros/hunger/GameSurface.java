package com.wnezros.hunger;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

public class GameSurface extends GLSurfaceView {
	
	GameRenderer _Renderer;
	
	public GameSurface(Context context) {
        super(context);
        
        setDebugFlags(DEBUG_CHECK_GL_ERROR);
        
        _Renderer = new GameRenderer();
        setRenderer(_Renderer);
    }
	
	public boolean onTouchEvent(final MotionEvent event) {
        switch(event.getActionMasked()) {
        case MotionEvent.ACTION_DOWN: {
        	int pid = event.getPointerId(0);
        	GameContext.mouseDown(pid, Float.valueOf(event.getX()).intValue(), Float.valueOf(event.getY()).intValue());
        } break;
        case MotionEvent.ACTION_POINTER_DOWN: {
        	int p = event.getActionIndex();
        	int pid = event.getPointerId(p);
        	GameContext.mouseDown(pid, Float.valueOf(event.getX(p)).intValue(), Float.valueOf(event.getY(p)).intValue());
        } break;
        case MotionEvent.ACTION_POINTER_UP: {
        	int p = event.getActionIndex();
        	int pid = event.getPointerId(p);
        	GameContext.mouseUp(pid, Float.valueOf(event.getX(p)).intValue(), Float.valueOf(event.getY(p)).intValue());
        } break;
        case MotionEvent.ACTION_MOVE: {
        	for(int p = 0; p < event.getPointerCount(); p++) {
	        	int pid = event.getPointerId(p);
	        	GameContext.mouseMove(Float.valueOf(event.getX(p)).intValue(), Float.valueOf(event.getY(p)).intValue(), pid);
        	}
        } break;
        case MotionEvent.ACTION_UP: {
        	int pid = event.getPointerId(0);
        	GameContext.mouseUp(pid, Float.valueOf(event.getX()).intValue(), Float.valueOf(event.getY()).intValue());
        } break;
        case MotionEvent.ACTION_CANCEL:
        	GameContext.mouseCancel();
        }
        return true;
    }
}

class GameRenderer implements GLSurfaceView.Renderer {
	boolean _created = false;
	@Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		if(_created) return;
		GameContext.init();
		_created = true;
    }

	@Override
    public void onSurfaceChanged(GL10 gl, int w, int h) {
		Log.i("SurfaceResize", "New size " + w + " x " + h);
        GameContext.resize(w, h);
    }

	@Override
    public void onDrawFrame(GL10 gl) {
        GameContext.render();
    }
}

