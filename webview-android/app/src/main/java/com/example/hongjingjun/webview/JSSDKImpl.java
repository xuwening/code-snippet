package com.example.hongjingjun.webview;

import android.webkit.JavascriptInterface;
import com.alibaba.fastjson.*;

class JSCallDetailRequest {

    public boolean debug;
    public String businessName;

    public String callbackSuccess;
    public String callbackError;
}

class JSCallDetailResponse {

    //.....
}


class JSSDKError {

    public int errCode;
    public String errDescrip;
}

/**
 * Created by hongjingjun on 16/3/9.
 */
public class JSSDKImpl implements JSSDKInterface {

    @JavascriptInterface
    public void getDocmentTitle(String title) {

        System.out.println(title);
    }


    @JavascriptInterface
    public void getCallDetail(String json) {

        JSCallDetailRequest request = JSON.parseObject(json, JSCallDetailRequest.class);
        if (request != null) {

            //获取详单数据，并填充到 response
            JSCallDetailResponse response = new JSCallDetailResponse();

//            if (获取数据成功) {
//
//                //转成 json 字符串
//                String res = JSON.toJSONString(response);
//                webview.loadUrl(request.callbackSuccess+ "(" + res + ")");
//
//                //toast 提示结果
//                if (request.debug) {   /*...  show toast  */ };
//            } else {
//
//                JSSDKError error = new JSSDKError();
//
//                //错误提示和错误描述
//                error.errCode = -1;
//                error.errDescrip = "error  info ...";
//                String res = JSON.toJSONString(error);
//                webview.loadUrl(request.callbackError + "(" + res + ")");
//
//                //toast 提示结果
//                if (request.debug) {   /*...  show toast  */ };
//            }
        }

    }


    @JavascriptInterface
    public void getUserInfo(String json) {

    }
}
