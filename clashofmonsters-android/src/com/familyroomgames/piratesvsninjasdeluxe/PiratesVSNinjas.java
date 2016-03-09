/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.familyroomgames.piratesvsninjasdeluxe;

import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;
import com.familyroomgames.piratesvsninjasdeluxe.R;
import com.google.analytics.tracking.android.EasyTracker;
import com.google.analytics.tracking.android.Fields;
import com.google.analytics.tracking.android.MapBuilder;
import com.google.analytics.tracking.android.Tracker;
import com.sromku.simple.fb.Permission;
import com.sromku.simple.fb.SimpleFacebookConfiguration;
import com.sromku.simple.fb.SimpleFacebook;
import com.sromku.simple.fb.listeners.OnLoginListener;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import com.chartboost.sdk.*;

import android.os.Bundle;
import android.util.Log;

import java.util.List;

public class PiratesVSNinjas extends Cocos2dxActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);

        Permission[] permissions = new Permission[]
        {
                Permission.PUBLISH_ACTION
        };

        SimpleFacebookConfiguration configuration = new SimpleFacebookConfiguration.Builder()
                .setAppId(getString(R.string.facebook_app_id))
                .setPermissions(permissions)
                .build();
        SimpleFacebook.setConfiguration(configuration);

        FacebookSdk.sdkInitialize(getApplicationContext());
        callbackManager = CallbackManager.Factory.create();
        shareDialog = new ShareDialog(this);
        // this part is optional
        shareDialog.registerCallback(callbackManager, new FacebookCallback<Sharer.Result>() {
            @Override
            public void onSuccess(Sharer.Result result) {
                facebookSharingDidFinish(1, "");
            }

            @Override
            public void onCancel() {
                facebookSharingDidFinish(0, "");
            }

            @Override
            public void onError(FacebookException error) {
                facebookSharingDidFinish(0, "");
            }
        });


        this.cb = Chartboost.sharedChartboost();
        String appId = "526e7b9a17ba47785700001d";
        String appSignature = "f7eafc49b297bd9cff3ad931fed2f88593742b65";
        this.cb.onCreate(this, appId, appSignature, null);
        this.cb.cacheInterstitial();

        initFacebookSharing(this);
        initChartboost(this);
        initGoogleAnalytics(this);
        initTutorial(this);
	}

    @Override
    protected void onStart()
    {
        super.onStart();

        this.cb.onStart(this);

        // Notify the beginning of a user session. Must not be dependent on user actions or any prior network requests.
        this.cb.startSession();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        simpleFacebook = SimpleFacebook.getInstance(this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        simpleFacebook.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onStop() {
        super.onStop();

        this.cb.onStop(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        this.cb.onDestroy(this);
    }

    @Override
    public void onBackPressed() {

        // If an interstitial is on screen, close it. Otherwise continue as normal.
        if (this.cb.onBackPressed())
            return;
        else
            super.onBackPressed();
    }

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// HelloCpp should create stencil buffer

        return glSurfaceView;
    }

    public void showInterstitial()
    {
        this.cb.showInterstitial();
    }

    public void trackScreenEnter(final String screenName)
    {
        // May return null if EasyTracker has not yet been initialized with a property ID.
        Tracker easyTracker = EasyTracker.getInstance(this);

        if (easyTracker != null)
        {
            // This screen name value will remain set on the tracker and sent with
            // hits until it is set to a new value or to null.
            easyTracker.set(Fields.SCREEN_NAME, "Home Screen");
            easyTracker.send(MapBuilder.createAppView().build());
        }
    }

    public void showTutorial()
    {
        final PiratesVSNinjas activity = this;
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                startActivity(new Intent(activity, TutorialScreenActivity.class));
            }
        });
    }

    public void facebookShare(final String text)
    {
        final PiratesVSNinjas activity = this;
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                final ProgressDialog progress = new ProgressDialog(activity);
                progress.setCancelable(false);
                progress.setProgressStyle(ProgressDialog.STYLE_SPINNER);
                progress.show();

                // login listener
                OnLoginListener onLoginListener = new OnLoginListener()
                {
                    @Override
                    public void onLogin(String accessToken, List<Permission> acceptedPermissions, List<Permission> declinedPermissions)
                    {
                        // change the state of the button or do whatever you want
                        Log.i("cocos2xd Pirates", "(PIRATES) Logged in");
                        Log.i("cocos2xd Pirates", "(PIRATES) Permissions: " + acceptedPermissions.toString());

                        // create publish listener
                        /*OnPublishListener onPublishListener = new OnPublishListener() {

                            @Override
                            public void onFail(String reason) {
                                // insure that you are logged in before publishing
                                Log.w("cocos2xd Pirates", "(PIRATES) " + reason);
                                progress.dismiss();
                                facebookSharingDidFinish(0, reason);
                            }

                            @Override
                            public void onException(Throwable throwable) {
                                Log.e("cocos2xd Pirates", "(PIRATES) Bad thing happened", throwable);
                                progress.dismiss();
                                facebookSharingDidFinish(0, "Facebook publish error");
                            }

                            @Override
                            public void onComplete(String postId) {
                                Log.i("cocos2xd Pirates", "(PIRATES) Published successfully. The new post id = " + postId);
                                progress.dismiss();
                                facebookSharingDidFinish(1, "");
                            }

                            @Override
                            public void onThinking() {
                            }
                        };

                        // build feed
                        Feed feed = new Feed.Builder()
                                .setName("Pirates VS Ninjas")
                                .setDescription(text)
                                .setPicture("https://photos-2.dropbox.com/t/2/AAC2_wVsTAooetyV_SbTXBC_LUIWPjouNYGn4jNwXIE3FA/12/498971797/png/32x32/3/1450263600/0/2/Pirates%20versus%20Ninjas.png/ENzlr4YEGBEgAigC/PiE7xKD7x15uL-KA6fYqEKIwNNFwsThARuiEFcUmikU?size_mode=5&size=32x32")
                                .setLink("https://play.google.com/store/apps/details?id=com.familyroomgames.piratesvsninjas")
                                .build();

                        // publish the feed
                        simpleFacebook.publish(feed, onPublishListener);  */

                        progress.dismiss();

                        if (ShareDialog.canShow(ShareLinkContent.class)) {
                            ShareLinkContent linkContent = new ShareLinkContent.Builder()
                                    .setContentTitle("Pirates VS Ninjas")
                                    .setContentDescription(text)
                                    .setContentUrl(Uri.parse("https://play.google.com/store/apps/details?id=com.familyroomgames.piratesvsninjas"))
                                    .build();

                            shareDialog.show(linkContent);
                        }
                    }

                    @Override
                    public void onFail(String reason)
                    {
                        Log.w("cocos2xd Pirates", "(PIRATES) " + reason);
                        progress.dismiss();
                        facebookSharingDidFinish(0, reason);
                    }

                    @Override
                    public void onException(Throwable throwable)
                    {
                        Log.e("cocos2xd Pirates", "(PIRATES) Bad thing happened", throwable);
                        progress.dismiss();
                        facebookSharingDidFinish(0, "Facebook login error");
                    }

                    @Override
                    public void onCancel() {
                        Log.e("cocos2xd Pirates", "(PIRATES) Cancel happened");
                        progress.dismiss();
                        facebookSharingDidFinish(0, "User cancelled login");
                    }
                };

                // login
                simpleFacebook.login(onLoginListener);
            }
        });

//        Session.openActiveSession(this, true, new Session.StatusCallback() {
//            @Override
//            public void call(Session session, SessionState state, Exception exception)
//            {
//                if (session.isOpened())
//                {
//
//                }
//            }
//        });
    }

    private static native void initFacebookSharing(PiratesVSNinjas activity);
    private static native void initChartboost(PiratesVSNinjas activity);
    private static native void initGoogleAnalytics(PiratesVSNinjas activity);
    private static native void initTutorial(PiratesVSNinjas activity);

    private static native void facebookSharingDidFinish(int success, String errorString);

    private CallbackManager callbackManager;
    private ShareDialog shareDialog;

    private SimpleFacebook simpleFacebook;
    private Chartboost cb;

    static {
        System.loadLibrary("piratesvsninjas");
        
        Log.d("pirates", "Loaded libpiratesvsninjas");
    }     
}
