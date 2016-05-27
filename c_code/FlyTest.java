package com.demo.test;

import java.util.Random;

import org.junit.Test;


public class FlyTest {

	@Test
	public void test() {
		//fail("Not yet implemented");
		
//		ProxyBook proxy = new ProxyBook();
//		proxy.proxy();
//		
		
		//根据imei号，将所有手机设备请求时间均匀打散在八小时之内，最小单位：毫秒
		int range = 8*60*60*1000;
		
		//同一imei号，每次生成时间都一致
		String imei = "XUIMLERF1LA02412K";
		ArangeStartTime.getRequestTime(imei, range);
		
		//针对无法获取的imei设备，随机生成imei号，然后打散
		imei = null;
		ArangeStartTime.getRequestTime(imei, range);
	}
}


class ArangeStartTime {
	
	private final static long prime = 16777619;
	private final static long offset = 2166136261L;
	private final static int imei_lenght = 15;
	
	
	private static long hashValue(String string) {
        long rv = offset;
        for (int i = 0; i < string.length(); i++) {
                rv *= prime;
                rv ^= string.charAt(i);
        }
        return rv;
	}
	
	private static String randomString(int lenght) {
		final String base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		
		Random random = new Random();
		StringBuffer result = new StringBuffer(lenght);
		for (int i=0; i<lenght; i++) {
			int idx = random.nextInt(base.length());
			result.append(base.charAt(idx));
		}
		
		return result.toString();
	}

	public static long getRequestTime(String imei, int range) {
		String hashstr = imei;
		if (hashstr == null) {
			hashstr = randomString(imei_lenght);
		}
		
		Random random = new Random(hashValue(hashstr));
		return random.nextInt(range);
	}
}





